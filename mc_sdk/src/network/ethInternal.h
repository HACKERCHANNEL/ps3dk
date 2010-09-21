/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * Copyright (C) 2007 Daniel Collin (daniel at collin dot com)
 */
/// This file contains enternal structs used by the EthDriver. Most of these aren't documented as the user will
/// not see these very often anyway.

#ifndef __ETHINTERNAL_H__
#define __ETHINTERNAL_H__ 1

#include <ps3types.h>
#include <ps3network.h>

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum 
{
	ETH_GDTDCEINT = 24,
	ETH_GRFANMINT = 28,
	ETH_GDADCEINT = 14,
	ETH_TXINT = (1L << (ETH_GDTDCEINT + 32)),

	ETH_RXINT0 = (1L << (ETH_GRFANMINT + 32)),
	ETH_RXINT1 = (1L << ETH_GDADCEINT),
	ETH_RXINT = (ETH_RXINT0 | ETH_RXINT1)

} ps3EthInterrupt;

typedef enum
{
	ETH_GET_MAC_ADDRESS = 1,
	ETH_GET_PORT_STATUS  =  2,
	ETH_SET_NEGOTIATION_MODE =  3,
	ETH_GET_VLAN_ID = 4,

} ps3EthStatus;

typedef enum
{
	ETH_LINK_UP      = 0x0001,
	ETH_FULL_DUPLEX  = 0x0002,
	ETH_AUTO_NEG     = 0x0004,
	ETH_SPEED_10     = 0x0010,
	ETH_SPEED_100    = 0x0020,
	ETH_SPEED_1000   = 0x0040,

} ps3EthLink;

typedef enum
{
	ETH_VLAN_ALL = 1,
	ETH_VLAN_WIRED,
	ETH_VLAN_WIRELESS,
	ETH_VLAN_PSP,
	ETH_VLAN_PORT0 = 0x10,
	ETH_VLAN_PORT1,
	ETH_VLAN_PORT2,
	ETH_VLAN_DAEMON_CLIENT_BSS,
	ETH_VLAN_LIBERO_CLIENT_BSS,
	ETH_VLAN_NO_ENTRY = -6,
	ETH_VLAN_POS = ETH_VLAN_ALEN * 2,
	ETH_VLAN_HLEN = 4

} ps3EthVlan;

typedef enum
{
	ETH_MAX_MTU = 2308,
	ETH_PORT = 2,
	ETH_RXBUF_ALIGN = 128,
	ETH_RX_CSUM_DEFAULT = 1,
	ETH_BROADCAST_ADDR = 0xffffffffffffL,
	ETH_MC_COUNT_MAX = 32, 
	ETH_P_8021Q	= 0x8100,    

} ps3EthMisc;

typedef enum
{
	ETH_DMAC_CMDSTAT_NOCS = 0xa0080000, 
	ETH_DMAC_CMDSTAT_TCPCS = 0xa00a0000,
	ETH_DMAC_CMDSTAT_UDPCS =  0xa00b0000,
	ETH_DMAC_CMDSTAT_END_FRAME = 0x00040000,

	ETH_DMAC_CMDSTAT_RXDCEIS = 0x00000002,

	ETH_DMAC_CMDSTAT_CHAIN_END =  0x00000002,
	ETH_DMAC_CMDSTAT_NOT_IN_USE = 0xb0000000,
	ETH_DESCR_IND_PROC_SHIFT = 28,
	ETH_DESCR_IND_PROC_MASKO = 0x0fffffff,

} ps3EthDmacCmd;

typedef enum 
{
	ETH_DESC_COMPLETE = 0, 
	ETH_DESC_RESPONSE_ERROR,
	ETH_DESC_PROTECTION_ERROR, 
	ETH_DESC_FRAME_END,
	ETH_DESC_FORCE_END, 
	ETH_DESC_CARDOWNED = 0x0a, 
	ETH_DESC_NOT_IN_USE,

} ps3EthDescStatus;

typedef struct 
{
	uint8_t dest[6];	   
	uint8_t source[6];
	uint16_t proto;  
	uint16_t tci;   
	uint16_t id;

} ps3EthVlanHeader;

typedef struct
{
	uint32_t address;
	uint32_t size;
	uint32_t nextDesc;
	uint32_t dmacStatus;
	uint32_t resultSize;
	uint32_t validSize;	
	uint32_t dataStatus;
	uint32_t dataError;

} ps3EthDriverDesc __attribute__((aligned(32)));


#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Move these to a more appropriate header

#define wmb()  __asm__ __volatile__ ("sync" : : : "memory")
#define PS3_ALIGN_UP(addr, size) (((addr) + ((size) - 1)) & (~((size) - 1)))
#define PS3_ALIGN_DOWN(addr, size) ((addr) & (~((size)-1)))
#define PS3_ALIGN(addr, size) PS3_ALIGN_UP(addr, size)
#define PS3_PAGE_ALIGN(addr) PS3_ALIGN(addr, PAGE_SIZE)
#define CPU2DMA_ADDR(a) (eth->dmaOffset + ps3RealToLpar(a))
#define DMA2CPU_ADDR(a) (ps3RealFromLpar((a) - eth->dmaOffset))

#endif
