/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2007 Daniel Collin (daniel at collin dot com)
// This file is subject to the terms and conditions of the ps3link License.
// See the file LICENSE in the main directory of this distribution for more
// details.
//

#include <ps3av.h>
#include <ps3gpu.h>
#include <ps3mmu.h>
#include <ps3console.h>
#include <ps3thread.h>
#include <ps3debug.h>
#include <limits.h>
#include <lwip/netif.h>
#include <lwip/ip.h>
#include <lwip/ip_frag.h>
#include <lwip/udp.h>
//#include <lwip/snmp_msg.h>
#include <lwip/tcp.h>
#include <lwip/memp.h>
#include <netif/etharp.h>

static struct ip_addr ipaddr, netmask, gw;
extern err_t ethifInit(struct netif* netif);
extern void echo_init();
uint8_t debug_flags = 0;
void ethInput(struct netif* netif);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	ps3AvVideoInit();
	ps3AvAudioInit();
	ps3AvAvInit(0);
	ps3AvAvbParamDualVideo(PS3AV_MODE_480I, PS3AV_MODE_480I);
	ps3GpuInit();
	
	ps3MmuInit();
	ps3MmuOn();
	ps3ConsoleInit();
	
	static struct netif netif;
	struct in_addr inaddr;
	char ip_str[16] = {0}, nm_str[16] = {0}, gw_str[16] = {0};

	/* startup defaults (may be overridden by one or more opts) */
	IP4_ADDR(&gw, 192,168,0,1);
	IP4_ADDR(&ipaddr, 192,168,0,8);
	IP4_ADDR(&netmask, 255,255,255,0);

	/* use debug flags defined by debug.h */
	debug_flags |= (LWIP_DBG_ON|LWIP_DBG_TRACE|LWIP_DBG_STATE|LWIP_DBG_FRESH|LWIP_DBG_HALT);
	
	inaddr.s_addr = ipaddr.addr;
	strncpy(ip_str,inet_ntoa(inaddr),sizeof(ip_str));
	inaddr.s_addr = netmask.addr;
	strncpy(nm_str,inet_ntoa(inaddr),sizeof(nm_str));
	inaddr.s_addr = gw.addr;
	strncpy(gw_str,inet_ntoa(inaddr),sizeof(gw_str));
	PS3_DEBUG("Host at %s mask %s gateway %s\n", ip_str, nm_str, gw_str);

	mem_init();
	memp_init();
	pbuf_init(); 
	netif_init();
	ip_init();
	udp_init();
	tcp_init();
	
	netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethifInit, ip_input);	
	netif_set_default(&netif);
	netif_set_up(&netif);

	echo_init();
/*	
	timer_init();
	timer_set_interval(TIMER_EVT_ETHARPTMR,2000);
	timer_set_interval(TIMER_EVT_TCPFASTTMR, TCP_FAST_INTERVAL / 10);
	timer_set_interval(TIMER_EVT_TCPSLOWTMR, TCP_SLOW_INTERVAL / 10);
#if IP_REASSEMBLY
	timer_set_interval(TIMER_EVT_IPREASSTMR,100);
#endif
*/	
	PS3_DEBUG("Applications started.\n");

	
	ethInput(&netif);
	ps3ThreadSleepMicros(2000);
	tcp_fasttmr();
	tcp_slowtmr();
	ip_reass_tmr();
	etharp_tmr();

	while (1) 
	{
		ethInput(&netif);
		//etharp_tmr();
	}
	
	return 0;
}

