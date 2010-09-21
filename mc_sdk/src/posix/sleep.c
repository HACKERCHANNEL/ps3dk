/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * sleep.c - POSIX compatibility function sleep()
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <unistd.h>
#include <ps3tb.h>
#include <ps3thread.h>

unsigned int sleep(unsigned int seconds)
{
  uint64_t t1, t0 = ps3TbRead();
  uint64_t tbPerSec = ps3TbHz();
  uint64_t tbDelay = seconds * tbPerSec;
  ps3ThreadSleepUntilInterruptable(t0 + tbDelay);
  t1 = ps3TbRead();
  if ((uint64_t)(t1 - t0) >= tbDelay)
    return 0;
  else
    return (tbDelay - (uint64_t)(t1 - t0)) / tbPerSec;
}

