/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * eth.c - Ethernet driver
 *
 * Copyright (C) 2007 Daniel Collin (daniel at collin dot com)
 */

#include <assert.h>
#include <string.h>
#include <ps3debug.h>
#include <ps3hv.h>
#include <ps3network.h>
#include "ethInternal.h"

static ps3Eth g_eth;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ps3Eth* ps3EthGetCurrent()
{
	return &g_eth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int setIrqMask(ps3Eth* desc, uint64_t mask)
{
	int status = lv1_net_set_interrupt_mask(desc->busId, desc->devId, mask, 0);

	if (status)
	{
		PS3_DEBUG("lv1_net_set_interrupt_mask %d\n", status);
	}

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int eventRecvPortSetup(const ps3Eth* desc, uint64_t cpu, unsigned int* irq)
{
	int result;
	uint64_t outlet;

	result = lv1_construct_event_receive_port(&outlet);

	if (result) 
	{
		PS3_DEBUG("lv1_construct_event_receive_port failed: %d\n", result);
		*irq = 0;
		return result;
	}

	// TODO: Proper irq id here

	*irq = 1;


	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void setDescStatus(ps3EthDriverDesc* desc, ps3EthDescStatus status)
{
	uint32_t newStatus;

	newStatus = desc->dmacStatus;
	newStatus &= ETH_DESCR_IND_PROC_MASKO;
	newStatus |= ((uint32_t)status) << ETH_DESCR_IND_PROC_SHIFT;
	desc->dmacStatus = newStatus;
	wmb();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ps3EthSendRawData(const void* data, unsigned int length)
{
	static ps3EthDriverDesc desc[2];
	static ps3EthVlanHeader vlan;
	const uint8_t* tempData = (const uint8_t*)data;
	int status = 0;
	unsigned int vlanLen = 0;
	unsigned int i = 0;

	assert(data);
	
	ps3Eth* eth = ps3EthGetCurrent();

	memcpy(&vlan.dest, &tempData[0], ETH_VLAN_ALEN);
	memcpy(&vlan.source, &tempData[6], ETH_VLAN_ALEN);

	if (eth->vlanIndex != -1) 
	{
		vlan.proto = ETH_P_8021Q; 
		vlan.tci = eth->vlanIds[eth->vlanIndex];
		vlanLen = ETH_VLAN_POS + ETH_VLAN_HLEN;
	} 
	else
	{
		vlanLen = ETH_VLAN_POS; 
	}

	tempData += 12;
	memset(desc, 0, sizeof(ps3EthDriverDesc) * 2);

	desc[0].address = CPU2DMA_ADDR(&vlan);
	desc[0].size = vlanLen;
	desc[0].nextDesc = CPU2DMA_ADDR(&desc[1]);
	desc[0].resultSize = 0;
	desc[0].dmacStatus = ETH_DMAC_CMDSTAT_NOCS;

	desc[1].address = CPU2DMA_ADDR(tempData);
	desc[1].size = length - 12;
	desc[1].nextDesc = 0;
	desc[1].validSize = 0;
	desc[1].dmacStatus = ETH_DMAC_CMDSTAT_NOCS | ETH_DMAC_CMDSTAT_END_FRAME;

	setDescStatus(&desc[0], ETH_DESC_CARDOWNED);
	setDescStatus(&desc[1], ETH_DESC_CARDOWNED);

	PS3_DEBUG("    ! %x %x\n", desc[0].dmacStatus,  desc[1].dmacStatus);
	status = lv1_net_start_tx_dma(eth->busId, eth->devId, CPU2DMA_ADDR(&desc[0]), 0);

	if (status)
	{
		PS3_DEBUG("lv1_net_start_tx_dma_bus: %d\n", status);
		return -1;
	}

	ps3ThreadSleepMicros(20);

	status = lv1_net_stop_tx_dma(eth->busId, eth->devId, 0);
	if (status)
	{
		PS3_DEBUG("lv1_net_stop_tx_dma_bus: %d\n", status);
		return -1;
	}

	PS3_DEBUG("sent! %x %x %x %x %x %x %x %x\n", 
			  desc[0].dmacStatus, desc[1].dmacStatus,
			  desc[0].resultSize, desc[1].resultSize, 
			  desc[0].dataStatus, desc[1].dataStatus,
			  desc[0].dataError, desc[1].dataError);

	// TODO: Validate that this size is correct

	return desc[1].resultSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ps3EthRecieveRawData(void* data)
{
	int status = 0;
	static ps3EthDriverDesc desc; 
	static uint8_t reciveBuffer[PS3_ALIGN(ETH_MAX_MTU, ETH_RXBUF_ALIGN)] __attribute__((aligned(128)));

	ps3Eth* eth = ps3EthGetCurrent();

	memset(reciveBuffer, 0, PS3_ALIGN(ETH_MAX_MTU, ETH_RXBUF_ALIGN));

	memset(&desc, 0, sizeof(desc));
	desc.size = PS3_ALIGN(ETH_MAX_MTU, ETH_RXBUF_ALIGN);
	desc.address = CPU2DMA_ADDR(reciveBuffer);
	desc.nextDesc = 0;
	setDescStatus(&desc, ETH_DESC_CARDOWNED);

	status = lv1_net_start_rx_dma(eth->busId, eth->devId, CPU2DMA_ADDR(&desc), 0);

	if (status)
	{
		PS3_DEBUG("lv1_net_start_tx_dma_bus: %d\n", status);
		return 0;
	}

	ps3ThreadSleepMicros(400);

	status = lv1_net_stop_rx_dma(eth->busId, eth->devId, 0);

	if (status)
	{
		PS3_DEBUG("lv1_net_stop_tx_dma_bus: %d\n", status);
		return 0;
	}

	if (reciveBuffer[0] != 0)
	{
		PS3_DEBUG("got! %x %d %d %d %d %d\n", 
				  desc.dmacStatus, desc.resultSize, desc.dataStatus, desc.dataError, desc.size, desc.validSize);
	}
	
	//PS3_DEBUG("res size %d\n", desc.resultSize);
	if (desc.validSize >= 2)
		memcpy(data, &reciveBuffer[2], desc.validSize - 2);

	return desc.resultSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int ethInit(unsigned int busId, unsigned int devId)
{
	uint64_t v1 = 0, v2 = 0;
	int i = 0;
	int status;

	ps3Eth* eth = ps3EthGetCurrent();
	eth->busId = busId;
	eth->devId = devId;

	status = lv1_open_device(busId, devId, 0);

	if (status)
	{
		PS3_DEBUG("lv1_open_device: %d\n", status);
		return -1;
	}

	status = lv1_net_control(busId, devId, 1 /* Mac address */, 0, 0, 0, &v1, &v2);

	if (status)
	{
		PS3_DEBUG("lv1_net_control: %d\n", status);
		return -1;
	}

	v1 <<= 16;
	memcpy(eth->macAddress, &v1, 6);

	PS3_DEBUG("MAC addr %x:%x:%x:%x:%x:%x\n",
			  eth->macAddress[0], eth->macAddress[1], eth->macAddress[2], 
			  eth->macAddress[3], eth->macAddress[4], eth->macAddress[5]);

	status = lv1_net_control(busId, devId, ETH_GET_PORT_STATUS, ETH_PORT, 0, 0, &v1, &v2);

	if (status)
	{
		PS3_DEBUG("lv1_net_control: %d\n", status);
		return -1;
	}

	if (v1 & ETH_LINK_UP)
		PS3_DEBUG("ETH_LINK_UP\n");

	if (v1 & ETH_FULL_DUPLEX)
		PS3_DEBUG("ETH_FULL_DUPLEX\n");

	if (v1 & ETH_AUTO_NEG)
		PS3_DEBUG("ETH_AUTO_NEG\n");

	if (v1 & ETH_SPEED_10)
		PS3_DEBUG("ETH_SPEED_10\n");

	if (v1 & ETH_SPEED_100)
		PS3_DEBUG("ETH_SPEED_100\n");

	if (v1 & ETH_SPEED_1000)
		PS3_DEBUG("ETH_SPEED_1000\n");

	eth->vlanIndex = -1;

	for (i = 0; i < ETH_VLAN_MAX; ++i) 
	{
		status = lv1_net_control(busId, devId, ETH_GET_VLAN_ID, i + 1, 0, 0, &v1, &v2);

		if (status == ETH_VLAN_NO_ENTRY) 
		{
			PS3_DEBUG("ETH_VLAN_ID no entry:%d, VLAN disabled\n", status);
			eth->vlanIds[i] = 0;
		} 
		else if (status) 
		{
			PS3_DEBUG("%s:ETH_VLAN_ID faild, status=%d\n", __func__, status);
			eth->vlanIds[i] = 0;
		} 
		else 
		{
			PS3_DEBUG("id:%d, %lx\n", i, v1);
			eth->vlanIds[i] = (uint32_t)v1;
		}
	}

	if (eth->vlanIds[ETH_VLAN_WIRED - 1])
		eth->vlanIndex = ETH_VLAN_WIRED - 1;

	PS3_DEBUG("%d\n", __LINE__);
	status = lv1_allocate_device_dma_region(busId, devId,
						ps3RealMemSize(), 16, 0,
						&eth->dmaOffset);
	if (status)
	{
		PS3_DEBUG("%d\n", __LINE__);
		PS3_DEBUG("lv1_allocate_device_dma_region: %d\n", status);
		return -1;
    }

	PS3_DEBUG("dmaRegion: 0x%lx %d\n", eth->dmaOffset, __LINE__);
    status = lv1_map_device_dma_region(busId, devId, 0, eth->dmaOffset,
				       ps3RealMemSize(), 0xf800000000000000UL);
	if (status)
	{
		PS3_DEBUG("%d\n", __LINE__);
		PS3_DEBUG("lv1_allocate_device_dma_region: %d\n", status);
		return -1;
	}

	status = lv1_net_set_interrupt_status_indicator(busId, devId, ps3RealToLpar(&eth->irqStatus), 0);

	if (status) 
	{
		PS3_DEBUG("lv1_net_set_interrupt_status_indicator failed: %d\n", status);
		return -1;
	}

	setIrqMask(eth, ETH_RXINT | ETH_TXINT);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ps3EthClose()
{
	ps3Eth* eth = ps3EthGetCurrent();
	lv1_close_device(eth->busId, eth->devId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Maybe move this to a more general bus finding functions?

int findBus(unsigned int busType, unsigned int* busId)
{
	int busIndex = 0;
	int foundBus = -1;
	uint64_t v1, v2;
	int64_t r;

	for (busIndex = 0; busIndex < 10; ++busIndex) 
	{
		PS3_DEBUG("Probe %d\n", busIndex);

		r = lv1_get_repository_node_value(1,
						  0x0000000062757300+busIndex, /* "bus" */
						  0x7479706500000000, /* "type" */
						  0, 0, &v1, &v2);
		if (r)
		{
			PS3_DEBUG("Probe failed %d\n", busIndex);
			continue;
		}

		PS3_DEBUG("foundBusType 0x%x\n", v1);

		if (busType != v1)
		{
			PS3_DEBUG("Not the type we want, look again\n");
			continue;
		}

		r = lv1_get_repository_node_value(1,
						  0x0000000062757300+busIndex, /* "bus" */
						  0x6964000000000000, /* "id" */
						  0, 0, &v1, &v2);
		if (r)
		{
			PS3_DEBUG("Probe failed %d\n", busIndex);
			continue;
		}

		*busId = (unsigned int)v1;

		PS3_DEBUG("Found busId: %d\n", *busId);
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ps3EthInit()
{
	uint64_t numDev;
	int result = 0;
	unsigned int busIndex;
	uint64_t deviceType;
	uint64_t deviceId;
	uint64_t i, v2;
	int64_t r;

	result = findBus(4 /*BUS_TYPE_SB*/, &busIndex);

	if (result)
	{
		PS3_DEBUG("findBus failed: %d\n", result);
		return -1;
	}

	result = lv1_get_repository_node_value(1,
				0x0000000062757300+busIndex, /* "bus" */
				0x6e756d5f64657600, /* "num_dev" */
				0, 0, &numDev, &v2);

	PS3_DEBUG("NumDevs to check %d\n", numDev);

	for (i = 0; i < numDev; ++i)
	{
		r = lv1_get_repository_node_value(1,
				0x0000000062757300+busIndex, /* "bus" */
				0x6465760000000000+i, /* "dev" */
				0x7479706500000000, /* "type" */
				0, &deviceType, &v2);
		if (r)
		{
			PS3_DEBUG("Probe failed %d\n testing next", i);
			continue;
		}
		
		
		r = lv1_get_repository_node_value(1,
				0x0000000062757300+busIndex, /* "bus" */
				0x6465760000000000+i, /* "dev" */
				0x6964000000000000, /* "id" */
				0, &deviceId, &v2);
		if (r)
		{
			PS3_DEBUG("Not the type we want, look again\n");
			continue;
		}

		PS3_DEBUG("FoundDevice %d %d\n", deviceType, deviceId);

		if (deviceType == 3 /*PS3_DEV_TYPE_SB_ETH*/)
		{
			return ethInit(busIndex, (unsigned int)deviceId);
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

