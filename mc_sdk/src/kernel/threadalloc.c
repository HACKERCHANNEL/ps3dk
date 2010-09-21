/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * threadalloc.c - Thread allocation
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3thread.h>
#include <stdlib.h>

#include "threadInternal.h"

ps3Thread_t ps3ThreadCreate(void (*func)(void *arg), void *arg,
			    uint64_t stack_size, uint8_t prio)
{
  struct ps3Thread *t = calloc(1, sizeof(struct ps3Thread)+stack_size);
  if (t) {
    t->priority = prio;
    ps3ThreadInit(t, func, arg, ((char *)(t+1))+stack_size);
    ps3ThreadAdd(t);
  }
  return t;
}

