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
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include <string.h>

#include "lwip/opt.h"

#include "lwip/def.h"

#include "lwip/stats.h"
#include "lwip/mem.h"


#if LWIP_STATS
struct stats_ lwip_stats;

void
stats_init(void)
{
  memset(&lwip_stats, 0, sizeof(struct stats_));
}
#if LWIP_STATS_DISPLAY
void
stats_display_proto(struct stats_proto *proto, char *name)
{
  LWIP_PLATFORM_DIAG(("\n%s\n\t", name));
  LWIP_PLATFORM_DIAG(("xmit: %"STAT_COUNTER_F"\n\t", proto->xmit)); 
  LWIP_PLATFORM_DIAG(("rexmit: %"STAT_COUNTER_F"\n\t", proto->rexmit)); 
  LWIP_PLATFORM_DIAG(("recv: %"STAT_COUNTER_F"\n\t", proto->recv)); 
  LWIP_PLATFORM_DIAG(("fw: %"STAT_COUNTER_F"\n\t", proto->fw)); 
  LWIP_PLATFORM_DIAG(("drop: %"STAT_COUNTER_F"\n\t", proto->drop)); 
  LWIP_PLATFORM_DIAG(("chkerr: %"STAT_COUNTER_F"\n\t", proto->chkerr)); 
  LWIP_PLATFORM_DIAG(("lenerr: %"STAT_COUNTER_F"\n\t", proto->lenerr)); 
  LWIP_PLATFORM_DIAG(("memerr: %"STAT_COUNTER_F"\n\t", proto->memerr)); 
  LWIP_PLATFORM_DIAG(("rterr: %"STAT_COUNTER_F"\n\t", proto->rterr)); 
  LWIP_PLATFORM_DIAG(("proterr: %"STAT_COUNTER_F"\n\t", proto->proterr)); 
  LWIP_PLATFORM_DIAG(("opterr: %"STAT_COUNTER_F"\n\t", proto->opterr)); 
  LWIP_PLATFORM_DIAG(("err: %"STAT_COUNTER_F"\n\t", proto->err)); 
  LWIP_PLATFORM_DIAG(("cachehit: %"STAT_COUNTER_F"\n", proto->cachehit)); 
}

void
stats_display_mem(struct stats_mem *mem, char *name)
{
  LWIP_PLATFORM_DIAG(("\n MEM %s\n\t", name));
  LWIP_PLATFORM_DIAG(("avail: %"MEM_SIZE_F"\n\t", mem->avail)); 
  LWIP_PLATFORM_DIAG(("used: %"MEM_SIZE_F"\n\t", mem->used)); 
  LWIP_PLATFORM_DIAG(("max: %"MEM_SIZE_F"\n\t", mem->max)); 
  LWIP_PLATFORM_DIAG(("err: %"MEM_SIZE_F"\n", mem->err));
  
}

void
stats_display(void)
{
#if MEMP_STATS
  s16_t i;
  char * memp_names[] = {
    "PBUF_REF/ROM",
    "RAW_PCB",
    "UDP_PCB",
    "TCP_PCB",
    "TCP_PCB_LISTEN",
    "TCP_SEG",
    "NETBUF",
    "NETCONN",
    "TCPIP_MSG",
#if ARP_QUEUEING
    "ARP_QUEUE",
#endif
    "PBUF_POOL",
    "SYS_TIMEOUT",
#if MEM_USE_POOLS
    "MEMP_MEM_POOL_1",
    "MEMP_MEM_POOL_2",
    "MEMP_MEM_POOL_3",
    "MEMP_MEM_POOL_4",
#endif
  };
#endif
#if LINK_STATS
  stats_display_proto(&lwip_stats.link, "LINK");
#endif
#if IPFRAG_STATS
  stats_display_proto(&lwip_stats.ip_frag, "IP_FRAG");
#endif
#if IP_STATS
  stats_display_proto(&lwip_stats.ip, "IP");
#endif
#if ICMP_STATS
  stats_display_proto(&lwip_stats.icmp, "ICMP");
#endif
#if UDP_STATS
  stats_display_proto(&lwip_stats.udp, "UDP");
#endif
#if TCP_STATS
  stats_display_proto(&lwip_stats.tcp, "TCP");
#endif
#if MEM_STATS
  stats_display_mem(&lwip_stats.mem, "HEAP");
#endif
#if MEMP_STATS
  for (i = 0; i < MEMP_MAX; i++) {
    stats_display_mem(&lwip_stats.memp[i], memp_names[i]);
  }
#endif
}
#endif /* LWIP_STATS_DISPLAY */
#endif /* LWIP_STATS */

