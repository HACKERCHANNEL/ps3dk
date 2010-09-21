#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include <ps3network.h>
#include <stdio.h>

/* Define those to better describe your network interface.*/
#define IFNAME0 'e'
#define IFNAME1 't'


static const struct eth_addr ethbroadcast = {{0xff,0xff,0xff,0xff,0xff,0xff}};
static err_t ps3EthifLowLevelOutput(struct netif* ethernetif, struct pbuf* p);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*  mintapif_init():
* 
*  Should be called at the beginning of the program to set up the
*  network interface. It calls the function low_level_init() to do the
*  actual setup of the hardware.
* 
*/

err_t ethifInit(struct netif* netif)
{
		ps3EthInit();

		netif->state = ps3EthGetCurrent();

#if LWIP_SNMP

		// ifType is other(1), there doesn't seem
		// to be a proper type for the tunnel if

		netif->link_type = 1;
		netif->link_speed = 0;
		netif->ts = 0;
		netif->ifinoctets = 0;
		netif->ifinucastpkts = 0;
		netif->ifinnucastpkts = 0;
		netif->ifindiscards = 0;
		netif->ifoutoctets = 0;
		netif->ifoutucastpkts = 0;
		netif->ifoutnucastpkts = 0;
		netif->ifoutdiscards = 0;
#endif

		//netif->ethaddr = (struct eth_addri*)&eth->macAddress[0];

		netif->hwaddr_len = 6;
		netif->name[0] = IFNAME0;
		netif->name[1] = IFNAME1;
		netif->output = etharp_output;
		netif->linkoutput = ps3EthifLowLevelOutput;
		netif->mtu = 1500;
		netif->hwaddr_len = 6;

		printf("netif->output: %p\n", netif->output);
		printf("netif->linkoutput: %p\n", netif->linkoutput);

		return ERR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*  ps3EthifLowLevelOutput():
* 
*  Should do the actual transmission of the packet. The packet is
*  contained in the pbuf that is passed to the function. This pbuf
*  might be chained.
* 
*/

static err_t ps3EthifLowLevelOutput(struct netif* netif, struct pbuf* p) 
{
	struct pbuf* q;
	uint8_t buffer[1600];
	uint8_t* ptr;

	//ps3Eth* eth = netif->state;

	if (p->tot_len >= 1600)
			return ERR_BUF;
	
	ptr=buffer;
	
	for (q = p; q != NULL; q = q->next) 
	{
		// Send the data from the pbuf to the interface, one pbuf at a
		//	 time. The size of the data in each pbuf is kept in the ->len
		// variable.
		// send data from(q->payload, q->len);

#ifdef NETIF_DEBUG
		//LWIP_DEBUGF(NETIF_DEBUG, ("netif: send ptr %x q->payload %x q->len %i q->next %x\n", 
		//						(int)ptr, (int)q->payload, (int)q->len, (int)q->next));
#endif
		memcpy(ptr, q->payload, q->len);
		ptr += q->len;
	}

	// signal that packet should be sent(); 

	// Need to do the actual write here, but first we need to find out how the data looks like
	// as we need to get the proper mac addresses

	PS3_DEBUG("Want to send stuff in size %d\n", p->tot_len);

	ps3EthSendRawData(buffer, p->tot_len);

	return ERR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*  low_level_input():
* 
*  Should allocate a pbuf and transfer the bytes of the incoming
*  packet from the interface into the pbuf.
* 
*/

static struct pbuf* ethLowLevelInput(struct netif* netif)
{
	struct pbuf* p;
	struct pbuf* q = 0;
	char* bufptr = 0;
	uint16_t len;
	char buf[1514];
	//ps3Eth* eth = netif->state;

	memset(buf, 0, 1514);

	// Obtain the size of the packet and put it into the "len" variable.
	len = ps3EthRecieveRawData(&buf);

	if (len == 0)
		return 0;

	// We allocate a pbuf chain of pbufs from the pool
	p = pbuf_alloc(PBUF_LINK, len, PBUF_POOL);
	snmp_add_ifinoctets(netif, len);
	
	if (p != NULL) 
	{
		// We iterate over the pbuf chain until we have read the entire packet into the pbuf
		bufptr = &buf[0];
		for (q = p; q != NULL; q = q->next) 
		{
			// Read enough bytes to fill this pbuf in the chain. The available data in the pbuf is given by the q->len variable
			// read data into(q->payload, q->len);
			memcpy(q->payload, bufptr, q->len);
			bufptr += q->len;
		}
		// acknowledge that packet has been read();
	} 
	else 
	{
		// drop packet(); 
		snmp_inc_ifindiscards(netif);
		PS3_DEBUG("Could not allocate pbufs\n");
	}

	return p;  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* 
*  This function should be called when a packet is ready to be read
*  from the interface. It uses the function low_level_input() that
*  should handle the actual reception of bytes from the network
*  interface.
* 
*/

void ethInput(struct netif* netif)
{
	struct eth_hdr* ethhdr;
	struct pbuf* p;
	ps3Eth* eth = netif->state;
	
	p = ethLowLevelInput(netif);

	if (p != NULL) 
	{

#if LINK_STATS
		lwip_stats.link.recv++;
#endif /* LINK_STATS */

		ethhdr = p->payload;

		switch (htons(ethhdr->type)) 
		{
			case ETHTYPE_IP:
			{
				//fb_clear(0xff00ff);
				//PS3_DEBUG("IP\n");
				// CSi disabled ARP table update on ingress IP packets.
	 			// This seems to work but needs thorough testing.
				// etharp_ip_input(netif, p);
				//printf("netif->input: 0x%lx\n %d\n", netif->input, __LINE__);
				//sleep(1);
				//printf("%s(%d)\n", __FUNCTION__, __LINE__);
				pbuf_header(p, -14);
				//printf("%s(%d)\n", __FUNCTION__, __LINE__);
				//sleep(1);
				netif->input(p, netif);
				//sleep(1);
				//printf("%d\n", __LINE__);
				break;
			}
			
			case ETHTYPE_ARP:
			{
				//PS3_DEBUG("ETHTYPE_ARP\n");
				netif->hwaddr_len = 6;
			    //printf("ethInput        : netif->linkoutput: 0x%lx 0x%lx %d\n", netif, netif->linkoutput, __LINE__);
				etharp_arp_input(netif, (struct eth_addr*)&eth->macAddress, p);
				break;
			}

			default:
			{
				//PS3_DEBUG("default %d\n", htons(ethhdr->type));
				LWIP_ASSERT("p != NULL", p != NULL);
				pbuf_free(p);
				break;
			}
		}
	}
}
/*-----------------------------------------------------------------------------------*/
/*w
int
mintapif_select(struct netif* netif)
{
	fd_set fdset;
	int ret;
	struct timeval tv;
	struct mintapif* mintapif;

	mintapif = netif->state;

	tv.tv_sec = 0;
	tv.tv_usec = 0; 
	
	FD_ZERO(&fdset);
	FD_SET(mintapif->fd, &fdset);

	ret = select(mintapif->fd + 1, &fdset, NULL, NULL, &tv);
	if (ret > 0) {
		mintapif_input(netif);	 
	}
	return ret;
}
*/



