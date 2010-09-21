/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3tb.h - Prototypes for timebase functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3TB_H__
#define __PS3TB_H__ 1

#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

static __inline__ uint64_t ps3TbRead()
{
  uint64_t tb;
  do __asm__ __volatile__("mftb %0" : "=r" (tb)); while(!tb);
  return tb;
}

extern uint64_t ps3TbHz();

#ifdef __cplusplus
}
#endif

#endif
