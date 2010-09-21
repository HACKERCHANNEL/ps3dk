/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * sleep_util.c - Convenience functions for sleeping amounts of time
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3thread.h>
#include <ps3tb.h>

void ps3ThreadSleep(uint64_t tbdelta)
{
  ps3ThreadSleepUntil(ps3TbRead() + tbdelta);
}

void ps3ThreadSleepSeconds(uint64_t seconds)
{
  ps3ThreadSleep(seconds * ps3TbHz());
}

void ps3ThreadSleepMillis(uint64_t millis)
{
  ps3ThreadSleep(millis * ps3TbHz() / 1000);
}

void ps3ThreadSleepMicros(uint64_t micros)
{
  ps3ThreadSleep(micros * ps3TbHz() / 1000000);
}
