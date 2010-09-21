/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * realmode.c - Real mode utilities
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3hv.h>
#include <ps3lpar.h>
#include <ps3realmode.h>

#define REALMODE_MASK 0x3FFFFFFFFFFFFFFFUL

static uint64_t realmodeLimit = 128 * 1024 * 1024;

static void getRealModeLimit(void) __attribute__ ((constructor));

static void getRealModeLimit(void)
{
  uint64_t puid, lpar_id, v2;

  if (!lv1_get_logical_pu_id(&puid) &&
      !lv1_get_logical_partition_id(&lpar_id))
    lv1_get_repository_node_value(lpar_id,
				  0x0000000062690000,	/* "bi" */
				  0x7075000000000000,	/* "pu" */
				  puid,			/* #<puid> */
				  0x726d5f73697a6500,	/* "rm_size" */
				  &realmodeLimit, &v2);
}

uint64_t ps3RealMemSize(void)
{
  return realmodeLimit;
}

lpar_addr_t ps3RealToLpar(const void *addr)
{
  lpar_addr_t lpar_addr = ((lpar_addr_t)addr) & REALMODE_MASK;
  if (lpar_addr < realmodeLimit)
    return lpar_addr;
  else
    return PS3_INVALID_LPAR_ADDR;
}

extern void *ps3RealFromLpar(lpar_addr_t addr)
{
  extern char _start[];
  if (addr < realmodeLimit)
    return (void *)(addr + (((uint64_t)_start) & ~REALMODE_MASK));
  else
    return 0;
}


/* This bypasses the versions in mmu.c iff we don't link with it */

lpar_addr_t ps3LparAddrFromPointer(const void *addr) __attribute__((weak, alias("ps3RealToLpar")));
void *ps3LparAddrToPointer(lpar_addr_t addr) __attribute__((weak, alias("ps3RealFromLpar")));
