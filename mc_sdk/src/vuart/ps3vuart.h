/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3vuart.h - Prototypes for the VUART library
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3VUART_H__
#define __PS3VUART_H__ 1

#include <sys/types.h>
#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PS3_VUART_UNIT_AV      0
#define PS3_VUART_UNIT_SYSMGR  2

extern ssize_t ps3VuartRead(int unit, unsigned char *data,
			    size_t length, unsigned timeout);

extern ssize_t ps3VuartWrite(int unit, const unsigned char *data,
			     size_t length);

#ifdef __cplusplus
}
#endif

#endif
