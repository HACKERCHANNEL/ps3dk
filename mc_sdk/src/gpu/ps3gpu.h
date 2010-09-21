/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3gpu.h - Prototypes for the GPU library
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3GPU_H__
#define __PS3GPU_H__ 1

#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int ps3GpuInit(void);
extern void *ps3GpuGetVramBase(void);
extern uint64_t ps3GpuGetVramSize(void);

#ifdef __cplusplus
}
#endif

#endif
