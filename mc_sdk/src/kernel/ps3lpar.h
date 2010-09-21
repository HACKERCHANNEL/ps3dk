/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3realmode.h - Prototypes for logical partition utilities
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3LPAR_H__
#define __PS3LPAR_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

extern lpar_addr_t ps3LparAddrFromPointer(const void *addr);
extern void *ps3LparAddrToPointer(lpar_addr_t addr);

#define PS3_INVALID_LPAR_ADDR (~0UL)

#ifdef __cplusplus
}
#endif

#endif
