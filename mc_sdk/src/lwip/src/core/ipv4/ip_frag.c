/* @file
 * 
 * This is the IP packet segmentation and reassembly implementation.
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Jani Monoses <jani@iv.ro> 
 * original reassembly code by Adam Dunkels <adam@sics.se>
 * 
 */

#include <string.h>

#include "lwip/opt.h"
#include "lwip/ip.h"
#include "lwip/ip_frag.h"
#include "lwip/netif.h"
#include "lwip/snmp.h"
#include "lwip/stats.h"

#if IP_REASSEMBLY
static u8_t ip_reassbuf[IP_HLEN + IP_REASS_BUFSIZE];
static u8_t ip_reassbitmap[IP_REASS_BUFSIZE / (8 * 8) + 1];
static const u8_t bitmap_bits[8] = { 0xff, 0x7f, 0x3f, 0x1f,
  0x0f, 0x07, 0x03, 0x01
};
static u16_t ip_reasslen;
static u8_t ip_reassflags;
#define IP_REASS_FLAG_LASTFRAG 0x01

static u8_t ip_reasstmr;
#endif /* IP_REASSEMBLY */

#if IP_REASSEMBLY || (IP_FRAG && IP_FRAG_USES_STATIC_BUF)
/*
 * Copy len bytes from offset in pbuf to buffer 
 *
 * helper used by both ip_reass and ip_frag
 *
 * @param p pbuf chain to copy from
 * @param offset offset in bytes into the pbuf chain which should be skipped
 *        before starting to copy
 * @param buffer destination to copy the data
 * @param len number of bytes to copy (starting from offset)
 * @return pointer to one pbuf in the chain p from which copied last
 */
static struct pbuf *
copy_from_pbuf(struct pbuf *p, u16_t *offset, u8_t *buffer, u16_t len)
{
  u16_t l;

  p->payload = (u8_t *)p->payload + *offset;
  p->len -= *offset;
  while (len) {
    l = len < p->len ? len : p->len;
    MEMCPY(buffer, p->payload, l);
    buffer += l;
    len -= l;
    if (len)
      p = p->next;
    else
      *offset = l;
  }
  return p;
}
#endif /* IP_REASSEMBLY || IP_FRAG_USES_STATIC_BUF */

#if IP_REASSEMBLY
/**
 * Initializes IP reassembly states.
 */
void
ip_reass_init(void)
{
  ip_reasstmr = 0;
  ip_reassflags = 0;
  ip_reasslen = 0;
  memset(ip_reassbitmap, 0, sizeof(ip_reassbitmap));
}

/**
 * Reassembly timer base function
 * for both NO_SYS == 0 and 1 (!).
 *
 * Should be called every 1000 msec.
 */
void
ip_reass_tmr(void)
{
  if (ip_reasstmr > 0) {
    ip_reasstmr--;
    LWIP_DEBUGF(IP_REASS_DEBUG, ("ip_reass_tmr: timer dec %"U16_F"\n",(u16_t)ip_reasstmr));
    if (ip_reasstmr == 0) {
      /* reassembly timed out */
      snmp_inc_ipreasmfails();
    }
  }
}

/**
 * Reassembles incoming IP fragments into an IP datagram.
 *
 * @param p points to a pbuf chain of the fragment
 * @return NULL if reassembly is incomplete, ? otherwise
 */
struct pbuf *
ip_reass(struct pbuf *p)
{
  struct pbuf *q;
  struct ip_hdr *fraghdr, *iphdr;
  u16_t offset, len;
  u16_t i;

  IPFRAG_STATS_INC(ip_frag.recv);
  snmp_inc_ipreasmreqds();

  iphdr = (struct ip_hdr *) ip_reassbuf;
  fraghdr = (struct ip_hdr *) p->payload;
  /* If ip_reasstmr is zero, no packet is present in the buffer, so we
     write the IP header of the fragment into the reassembly
     buffer. The timer is updated with the maximum age. */
  if (ip_reasstmr == 0) {
    LWIP_DEBUGF(IP_REASS_DEBUG, ("ip_reass: new packet\n"));
    SMEMCPY(iphdr, fraghdr, IP_HLEN);
    ip_reasstmr = IP_REASS_MAXAGE;
    ip_reassflags = 0;
    /* Clear the bitmap. */
    memset(ip_reassbitmap, 0, sizeof(ip_reassbitmap));
  }

  /* Check if the incoming fragment matches the one currently present
     in the reasembly buffer. If so, we proceed with copying the
     fragment into the buffer. */
  if (ip_addr_cmp(&iphdr->src, &fraghdr->src) &&
      ip_addr_cmp(&iphdr->dest, &fraghdr->dest) &&
      IPH_ID(iphdr) == IPH_ID(fraghdr)) {
    LWIP_DEBUGF(IP_REASS_DEBUG, ("ip_reass: matching previous fragment ID=%"X16_F"\n",
      ntohs(IPH_ID(fraghdr))));
    IPFRAG_STATS_INC(ip_frag.cachehit);
    /* Find out the offset in the reassembly buffer where we should
       copy the fragment. */
    len = ntohs(IPH_LEN(fraghdr)) - IPH_HL(fraghdr) * 4;
    offset = (ntohs(IPH_OFFSET(fraghdr)) & IP_OFFMASK) * 8;

    /* If the offset or the offset + fragment length overflows the
       reassembly buffer, we discard the entire packet. */
    if ((offset > IP_REASS_BUFSIZE) || ((offset + len) > IP_REASS_BUFSIZE)) {
      LWIP_DEBUGF(IP_REASS_DEBUG,
       ("ip_reass: fragment outside of buffer (%"S16_F":%"S16_F"/%"S16_F").\n", offset,
        offset + len, IP_REASS_BUFSIZE));
      ip_reasstmr = 0;
      snmp_inc_ipreasmfails();
      goto nullreturn;
    }

    /* Copy the fragment into the reassembly buffer, at the right
       offset. */
    LWIP_DEBUGF(IP_REASS_DEBUG,
     ("ip_reass: copying with offset %"S16_F" into %"S16_F":%"S16_F"\n", offset,
      IP_HLEN + offset, IP_HLEN + offset + len));
    i = IPH_HL(fraghdr) * 4;
    copy_from_pbuf(p, &i, &ip_reassbuf[IP_HLEN + offset], len);

    /* Update the bitmap. */
    if (offset / (8 * 8) == (offset + len) / (8 * 8)) {
      LWIP_DEBUGF(IP_REASS_DEBUG,
       ("ip_reass: updating single byte in bitmap.\n"));
      /* If the two endpoints are in the same byte, we only update that byte. */
      LWIP_ASSERT("offset / (8 * 8) < sizeof(ip_reassbitmap)",
                   offset / (8 * 8) < sizeof(ip_reassbitmap));
      ip_reassbitmap[offset / (8 * 8)] |=
        bitmap_bits[(offset / 8) & 7] &
        ~bitmap_bits[((offset + len) / 8) & 7];
    } else {
      /* If the two endpoints are in different bytes, we update the
         bytes in the endpoints and fill the stuff inbetween with
         0xff. */
      LWIP_ASSERT("offset / (8 * 8) < sizeof(ip_reassbitmap)",
                   offset / (8 * 8) < sizeof(ip_reassbitmap));
      ip_reassbitmap[offset / (8 * 8)] |= bitmap_bits[(offset / 8) & 7];
      LWIP_DEBUGF(IP_REASS_DEBUG,
       ("ip_reass: updating many bytes in bitmap (%"S16_F":%"S16_F").\n",
        1 + offset / (8 * 8), (offset + len) / (8 * 8)));
      for (i = 1 + offset / (8 * 8); i < (offset + len) / (8 * 8); ++i) {
        ip_reassbitmap[i] = 0xff;
      }
      LWIP_ASSERT("(offset + len) / (8 * 8) < sizeof(ip_reassbitmap)",
                   (offset + len) / (8 * 8) < sizeof(ip_reassbitmap));
      ip_reassbitmap[(offset + len) / (8 * 8)] |=
        ~bitmap_bits[((offset + len) / 8) & 7];
    }

    /* If this fragment has the More Fragments flag set to zero, we
       know that this is the last fragment, so we can calculate the
       size of the entire packet. We also set the
       IP_REASS_FLAG_LASTFRAG flag to indicate that we have received
       the final fragment. */

    if ((ntohs(IPH_OFFSET(fraghdr)) & IP_MF) == 0) {
      ip_reassflags |= IP_REASS_FLAG_LASTFRAG;
      ip_reasslen = offset + len;
      LWIP_DEBUGF(IP_REASS_DEBUG,
       ("ip_reass: last fragment seen, total len %"S16_F"\n",
        ip_reasslen));
    }

    /* Finally, we check if we have a full packet in the buffer. We do
       this by checking if we have the last fragment and if all bits
       in the bitmap are set. */
    if (ip_reassflags & IP_REASS_FLAG_LASTFRAG) {
      /* Check all bytes up to and including all but the last byte in
         the bitmap. */
      LWIP_ASSERT("ip_reasslen / (8 * 8) - 1 < sizeof(ip_reassbitmap)",
                   ip_reasslen / (8 * 8) - 1 < sizeof(ip_reassbitmap));
      for (i = 0; i < ip_reasslen / (8 * 8) - 1; ++i) {
        if (ip_reassbitmap[i] != 0xff) {
          LWIP_DEBUGF(IP_REASS_DEBUG,
           ("ip_reass: last fragment seen, bitmap %"S16_F"/%"S16_F" failed (%"X16_F")\n",
            i, ip_reasslen / (8 * 8) - 1, ip_reassbitmap[i]));
          goto nullreturn;
        }
      }
      /* Check the last byte in the bitmap. It should contain just the
         right amount of bits. */
      LWIP_ASSERT("ip_reasslen / (8 * 8) < sizeof(ip_reassbitmap)",
                   ip_reasslen / (8 * 8) < sizeof(ip_reassbitmap));
      if (ip_reassbitmap[ip_reasslen / (8 * 8)] !=
        (u8_t) ~ bitmap_bits[ip_reasslen / 8 & 7]) {
         LWIP_DEBUGF(IP_REASS_DEBUG,
          ("ip_reass: last fragment seen, bitmap %"S16_F" didn't contain %"X16_F" (%"X16_F")\n",
        ip_reasslen / (8 * 8), ~bitmap_bits[ip_reasslen / 8 & 7],
        ip_reassbitmap[ip_reasslen / (8 * 8)]));
        goto nullreturn;
      }

      /* Pretend to be a "normal" (i.e., not fragmented) IP packet
         from now on. */
      ip_reasslen += IP_HLEN;

      IPH_LEN_SET(iphdr, htons(ip_reasslen));
      IPH_OFFSET_SET(iphdr, 0);
      IPH_CHKSUM_SET(iphdr, 0);
      IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, IP_HLEN));

      /* If we have come this far, we have a full packet in the
         buffer, so we allocate a pbuf and copy the packet into it. We
         also reset the timer. */
      ip_reasstmr = 0;
      pbuf_free(p);
      p = pbuf_alloc(PBUF_LINK, ip_reasslen, PBUF_POOL);
      if (p != NULL) {
        i = 0;
        for (q = p; q != NULL; q = q->next) {
          /* Copy enough bytes to fill this pbuf in the chain. The
             available data in the pbuf is given by the q->len variable. */
          LWIP_DEBUGF(IP_REASS_DEBUG,
           ("ip_reass: memcpy from %p (%"S16_F") to %p, %"S16_F" bytes\n",
            (void *)&ip_reassbuf[i], i, q->payload,
            q->len > ip_reasslen - i ? ip_reasslen - i : q->len));
          MEMCPY(q->payload, &ip_reassbuf[i],
            q->len > ip_reasslen - i ? ip_reasslen - i : q->len);
          i += q->len;
        }
        IPFRAG_STATS_INC(ip_frag.fw);
        snmp_inc_ipreasmoks();
      } else {
        LWIP_DEBUGF(IP_REASS_DEBUG,
          ("ip_reass: pbuf_alloc(PBUF_LINK, ip_reasslen=%"U16_F", PBUF_POOL) failed\n", ip_reasslen));
        IPFRAG_STATS_INC(ip_frag.memerr);
        snmp_inc_ipreasmfails();
      }
      LWIP_DEBUGF(IP_REASS_DEBUG, ("ip_reass: p %p\n", (void*)p));
      return p;
    }
  }

nullreturn:
  IPFRAG_STATS_INC(ip_frag.drop);
  pbuf_free(p);
  return NULL;
}
#endif /* IP_REASSEMBLY */

#if IP_FRAG
#if IP_FRAG_USES_STATIC_BUF
static u8_t buf[LWIP_MEM_ALIGN_SIZE(IP_FRAG_MAX_MTU)];
#endif /* IP_FRAG_USES_STATIC_BUF */

/**
 * Fragment an IP datagram if too large for the netif.
 *
 * Chop the datagram in MTU sized chunks and send them in order
 * by using a fixed size static memory buffer (PBUF_REF) or
 * point PBUF_REFs into p (depending on IP_FRAG_USES_STATIC_BUF).
 *
 * @param p ip packet to send
 * @param netif the netif on which to send
 * @param dest destination ip address to which to send
 *
 * @return ERR_OK if sent successfully, err_t otherwise
 */
err_t 
ip_frag(struct pbuf *p, struct netif *netif, struct ip_addr *dest)
{
  struct pbuf *rambuf;
#if IP_FRAG_USES_STATIC_BUF
  struct pbuf *header;
#else
  struct pbuf *newpbuf;
  struct ip_hdr *original_iphdr;
#endif
  struct ip_hdr *iphdr;
  u16_t nfb;
  u16_t left, cop;
  u16_t mtu = netif->mtu;
  u16_t ofo, omf;
  u16_t last;
  u16_t poff = IP_HLEN;
  u16_t tmp;
#if !IP_FRAG_USES_STATIC_BUF
  u16_t newpbuflen, left_to_copy;
#endif

  /* Get a RAM based MTU sized pbuf */
#if IP_FRAG_USES_STATIC_BUF
  /* When using a static buffer, we use a PBUF_REF, which we will
   * use to reference the packet (without link header).
   * Layer and length is irrelevant.
   */
  rambuf = pbuf_alloc(PBUF_LINK, 0, PBUF_REF);
  if (rambuf == NULL) {
    LWIP_DEBUGF(IP_REASS_DEBUG, ("ip_frag: pbuf_alloc(PBUF_LINK, 0, PBUF_REF) failed\n"));
    return ERR_MEM;
  }
  rambuf->tot_len = rambuf->len = mtu;
  rambuf->payload = LWIP_MEM_ALIGN((void *)buf);

  /* Copy the IP header in it */
  iphdr = rambuf->payload;
  SMEMCPY(iphdr, p->payload, IP_HLEN);
#else /* IP_FRAG_USES_STATIC_BUF */
  original_iphdr = p->payload;
  iphdr = original_iphdr;
#endif /* IP_FRAG_USES_STATIC_BUF */

  /* Save original offset */
  tmp = ntohs(IPH_OFFSET(iphdr));
  ofo = tmp & IP_OFFMASK;
  omf = tmp & IP_MF;

  left = p->tot_len - IP_HLEN;

  nfb = (mtu - IP_HLEN) / 8;

  while (left) {
    last = (left <= mtu - IP_HLEN);

    /* Set new offset and MF flag */
    tmp = omf | (IP_OFFMASK & (ofo));
    if (!last)
      tmp = tmp | IP_MF;

    /* Fill this fragment */
    cop = last ? left : nfb * 8;

#if IP_FRAG_USES_STATIC_BUF
    p = copy_from_pbuf(p, &poff, (u8_t *) iphdr + IP_HLEN, cop);
#else /* IP_FRAG_USES_STATIC_BUF */
    /* When not using a static buffer, create a chain of pbufs.
     * The first will be a PBUF_RAM holding the link and IP header.
     * The rest will be PBUF_REFs mirroring the pbuf chain to be fragged,
     * but limited to the size of an mtu.
     */
    rambuf = pbuf_alloc(PBUF_LINK, IP_HLEN, PBUF_RAM);
    if (rambuf == NULL) {
      return ERR_MEM;
    }
    LWIP_ASSERT("this needs a pbuf in one piece!",
                (p->len >= (IP_HLEN)));
    SMEMCPY(rambuf->payload, original_iphdr, IP_HLEN);
    iphdr = rambuf->payload;

    /* Can just adjust p directly for needed offset. */
    p->payload = (u8_t *)p->payload + poff;
    p->len -= poff;

    left_to_copy = cop;
    while (left_to_copy) {
      newpbuflen = (left_to_copy < p->len) ? left_to_copy : p->len;
      /* Is this pbuf already empty? */
      if (!newpbuflen) {
        p = p->next;
        continue;
      }
      newpbuf = pbuf_alloc(PBUF_RAW, 0, PBUF_REF);
      if (newpbuf == NULL) {
        pbuf_free(rambuf);
        return ERR_MEM;
      }
      /* Mirror this pbuf, although we might not need all of it. */
      newpbuf->payload = p->payload;
      newpbuf->len = newpbuf->tot_len = newpbuflen;
      /* Add it to end of rambuf's chain, but using pbuf_cat, not pbuf_chain
       * so that it is removed when pbuf_dechain is later called on rambuf.
       */
      pbuf_cat(rambuf, newpbuf);
      left_to_copy -= newpbuflen;
      if (left_to_copy)
        p = p->next;
    }
    poff = newpbuflen;
#endif /* IP_FRAG_USES_STATIC_BUF */

    /* Correct header */
    IPH_OFFSET_SET(iphdr, htons(tmp));
    IPH_LEN_SET(iphdr, htons(cop + IP_HLEN));
    IPH_CHKSUM_SET(iphdr, 0);
    IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, IP_HLEN));

#if IP_FRAG_USES_STATIC_BUF
    if (last)
      pbuf_realloc(rambuf, left + IP_HLEN);

    /* This part is ugly: we alloc a RAM based pbuf for 
     * the link level header for each chunk and then 
     * free it.A PBUF_ROM style pbuf for which pbuf_header
     * worked would make things simpler.
     */
    header = pbuf_alloc(PBUF_LINK, 0, PBUF_RAM);
    if (header != NULL) {
      pbuf_chain(header, rambuf);
      netif->output(netif, header, dest);
      IPFRAG_STATS_INC(ip_frag.xmit);
      snmp_inc_ipfragcreates();
      pbuf_free(header);
    } else {
      LWIP_DEBUGF(IP_REASS_DEBUG, ("ip_frag: pbuf_alloc() for header failed\n"));
      pbuf_free(rambuf);      
      return ERR_MEM;    
    }
#else /* IP_FRAG_USES_STATIC_BUF */
    /* No need for separate header pbuf - we allowed room for it in rambuf
     * when allocated.
     */
    netif->output(netif, rambuf, dest);
    IPFRAG_STATS_INC(ip_frag.xmit);

    /* Unfortunately we can't reuse rambuf - the hardware may still be
     * using the buffer. Instead we free it (and the ensuing chain) and
     * recreate it next time round the loop. If we're lucky the hardware
     * will have already sent the packet, the free will really free, and
     * there will be zero memory penalty.
     */
    
    pbuf_free(rambuf);
#endif /* IP_FRAG_USES_STATIC_BUF */
    left -= cop;
    ofo += nfb;
  }
#if IP_FRAG_USES_STATIC_BUF
  pbuf_free(rambuf);
#endif /* IP_FRAG_USES_STATIC_BUF */
  snmp_inc_ipfragoks();
  return ERR_OK;
}
#endif /* IP_FRAG */
