/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * sleep.c - Time limited suspension of a thread
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3thread.h>
#include <ps3tb.h>

#include "threadInternal.h"

void ps3ThreadSleepUntil(uint64_t tbval)
{
  if ( (int64_t)(ps3TbRead() - tbval) < 0 )
    ps3ThreadSuspendTimeout(tbval);
}

void ps3ThreadSleepUntilInterruptable(uint64_t tbval)
{
  ps3ThreadSleepUntil(tbval);
}
