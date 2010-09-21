/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3realmode.h - Prototypes for real mode addressing utilities
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3REALMODE_H__
#define __PS3REALMODE_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

extern uint64_t ps3RealMemSize(void);
extern lpar_addr_t ps3RealToLpar(const void *addr);
extern void *ps3RealFromLpar(lpar_addr_t);

#ifdef __cplusplus
}
#endif

#endif
