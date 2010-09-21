/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3types.h - Commonly used typedefs.
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3TYPES_H__
#define __PS3TYPES_H__ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/* A logical partition address */

typedef uint64_t lpar_addr_t;


/* A DMA address */

typedef uint64_t dma_addr_t;


/* A handle for a GPU memory block */

typedef uint64_t gpu_memory_handle_t;


/* A handle for a GPU context */

typedef uint64_t gpu_context_handle_t;


/* An atomic value */
typedef uint32_t atomic_t;


#ifdef __cplusplus
}
#endif

#endif /* __PS3TYPES_H__ */
