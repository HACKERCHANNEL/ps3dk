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
#ifndef __LWIP_API_H__
#define __LWIP_API_H__

#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"

#include "lwip/ip.h"

#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Throughout this file, IP addresses and port numbers are expected to be in
 * the same byte order as in the corresponding pcb.
 */

#define NETCONN_NOCOPY 0x00
#define NETCONN_COPY   0x01

/* Helpers to process several netconn_types by the same code */
#define NETCONNTYPE_GROUP(t)    (t&0xF0)
#define NETCONNTYPE_DATAGRAM(t) (t&0xE0)

enum netconn_type {
  NETCONN_INVALID    = 0,
  /* NETCONN_TCP Group */
  NETCONN_TCP        = 0x10,
  /* NETCONN_UDP Group */
  NETCONN_UDP        = 0x20,
  NETCONN_UDPLITE    = 0x21,
  NETCONN_UDPNOCHKSUM= 0x22,
  /* NETCONN_RAW Group */
  NETCONN_RAW        = 0x40
};

enum netconn_state {
  NETCONN_NONE,
  NETCONN_WRITE,
  NETCONN_ACCEPT,
  NETCONN_RECV,
  NETCONN_CONNECT,
  NETCONN_CLOSE
};

enum netconn_evt {
  NETCONN_EVT_RCVPLUS,
  NETCONN_EVT_RCVMINUS,
  NETCONN_EVT_SENDPLUS,
  NETCONN_EVT_SENDMINUS
};

#if LWIP_IGMP
enum netconn_igmp {
  NETCONN_JOIN,
  NETCONN_LEAVE
};
#endif /* LWIP_IGMP */

struct netbuf {
  struct pbuf *p, *ptr;
  struct ip_addr *addr;
  u16_t port;
};

struct netconn {
  enum netconn_type type;
  enum netconn_state state;
  union {
    struct tcp_pcb *tcp;
    struct udp_pcb *udp;
    struct raw_pcb *raw;
  } pcb;
  err_t err;
  sys_mbox_t mbox;
  sys_mbox_t recvmbox;
  sys_mbox_t acceptmbox;
  int socket;
#if LWIP_SO_RCVTIMEO
  int recv_timeout;
#endif /* LWIP_SO_RCVTIMEO */
  u16_t recv_avail;
  /** TCP: when data passed to netconn_write doesn't fit into the send buffer,
      this temporarily stores the message. */
  struct api_msg_msg *write_msg;
  /** TCP: when data passed to netconn_write doesn't fit into the send buffer,
      this temporarily stores how much is already sent. */
  int write_offset;
#if LWIP_TCPIP_CORE_LOCKING
  /** TCP: when data passed to netconn_write doesn't fit into the send buffer,
      this temporarily stores whether to wake up the original application task
      if data couldn't be sent in the first try. */
  u8_t write_delayed;
#endif

  void (* callback)(struct netconn *, enum netconn_evt, u16_t len);
};

/* Network buffer functions: */
struct netbuf *   netbuf_new      (void);
void              netbuf_delete   (struct netbuf *buf);
void *            netbuf_alloc    (struct netbuf *buf, u16_t size);
void              netbuf_free     (struct netbuf *buf);
err_t             netbuf_ref      (struct netbuf *buf,
           const void *dataptr, u16_t size);
void              netbuf_chain    (struct netbuf *head,
           struct netbuf *tail);

u16_t             netbuf_len      (struct netbuf *buf);
err_t             netbuf_data     (struct netbuf *buf,
           void **dataptr, u16_t *len);
s8_t              netbuf_next     (struct netbuf *buf);
void              netbuf_first    (struct netbuf *buf);

void              netbuf_copy_partial(struct netbuf *buf, void *dataptr, 
              u16_t len, u16_t offset);

#define netbuf_copy(buf,dataptr,len) netbuf_copy_partial(buf, dataptr, len, 0)
#define netbuf_len(buf)              ((buf)->p->tot_len)
#define netbuf_fromaddr(buf)         ((buf)->addr)
#define netbuf_fromport(buf)         ((buf)->port)

/* Network connection functions: */
#define netconn_new(t)                  netconn_new_with_proto_and_callback(t, 0, NULL)
#define netconn_new_with_callback(t, c) netconn_new_with_proto_and_callback(t, 0, c)
struct
netconn *netconn_new_with_proto_and_callback(enum netconn_type t, u8_t proto,
                                   void (*callback)(struct netconn *, enum netconn_evt, u16_t len));
err_t             netconn_delete  (struct netconn *conn);
enum netconn_type netconn_type    (struct netconn *conn);
err_t             netconn_peer    (struct netconn *conn,
           struct ip_addr *addr,
           u16_t *port);
err_t             netconn_addr    (struct netconn *conn,
           struct ip_addr **addr,
           u16_t *port);
err_t             netconn_bind    (struct netconn *conn,
           struct ip_addr *addr,
           u16_t port);
err_t             netconn_connect (struct netconn *conn,
           struct ip_addr *addr,
           u16_t port);
err_t             netconn_disconnect (struct netconn *conn);
err_t             netconn_listen  (struct netconn *conn);
struct netconn *  netconn_accept  (struct netconn *conn);
struct netbuf *   netconn_recv    (struct netconn *conn);
err_t             netconn_sendto  (struct netconn *conn,
           struct netbuf *buf, struct ip_addr *addr, u16_t port);
err_t             netconn_send    (struct netconn *conn,
           struct netbuf *buf);
err_t             netconn_write   (struct netconn *conn,
           const void *dataptr, int size,
           u8_t copy);
err_t             netconn_close   (struct netconn *conn);

#if LWIP_IGMP
err_t             netconn_join_leave_group (struct netconn *conn,
                                            struct ip_addr *multiaddr,
                                            struct ip_addr *interface,
                                            enum netconn_igmp join_or_leave);
#endif /* LWIP_IGMP */

#define netconn_err(conn)         ((conn)->err)

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_API_H__ */
