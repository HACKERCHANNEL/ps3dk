/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3network.h - Prototypes for the networking library
 *
 * Copyright (C) 2007 Daniel Collin (daniel at collin dot com)
 */
#ifndef __PS3NETWORK_H__
#define __PS3NETWORK_H__ 1

#include <ps3types.h>

#ifdef __cplusplus
extern "C" 
{
#endif

enum
{
	ETH_VLAN_MAX = 4,
	ETH_VLAN_ALEN = 6,
};

typedef struct
{
	uint64_t irqStatus;
        dma_addr_t dmaOffset;
	uint32_t vlanIds[ETH_VLAN_MAX];
	uint32_t vlanIndex;
	uint32_t busId;
	uint32_t devId;
	uint8_t macAddress[ETH_VLAN_ALEN];

} ps3Eth;

/**
 * Initializes the PS3 Network Interface.
 *
 * @return A non-negative value indicates success, a negative value is an error
**/
int ps3EthInit();

/**
 * Closes down the PS3 Network Interface.
 *
 * @return (void)
**/
void ps3EthClose();

/**
 * Sends raw data over the network, the sender is responsible to fill in correct mac addresses etc.
 *
 * @param data - data to be sent 
 * @param length - length of the data 
 * @return length of the the data that was sent, a negative value on error
**/
int ps3EthSendRawData(const void* data, unsigned int length);

/**
 * Recieve data over the network. The data given back is a typical ethernet frame.
 *
 * @param data - to be filed, max size is a ethenet frame - 1536 bytes. 
 *               The code will assume that the buffer is at least this size.
 * @return length of the the data that was recieved, a negative value on error
**/
int ps3EthRecieveRawData(void* data);

/**
 * Get the internal structure with info used by the Ethernet driver
 *
 * @return pointer to a PdkEth sturcture, 0 indicates error
**/
ps3Eth* ps3EthGetCurrent();

#ifdef __cplusplus
}
#endif

#endif

