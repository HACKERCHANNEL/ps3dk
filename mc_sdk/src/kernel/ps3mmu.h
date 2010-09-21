/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3mmu.h - Prototypes for memory management functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3MMU_H__
#define __PS3MMU_H__ 1

#include <sys/types.h>
#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void ps3MmuInit(void);
extern void ps3MmuTerm(void);
extern void ps3MmuOn(void);
extern void ps3MmuOff(void);
extern void *ps3MmuIoRemap(lpar_addr_t addr, uint64_t size);
extern void ps3MmuSetupSlb(void);
extern lpar_addr_t ps3MmuVirtToLpar(const void *addr);
extern void *ps3MmuLparToVirt(lpar_addr_t);

#ifdef __cplusplus
}
#endif

#endif
