/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * usleep.c - POSIX compatibility function usleep()
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <unistd.h>
#include <ps3tb.h>
#include <ps3thread.h>

int usleep(useconds_t usec)
{
  uint64_t tbPerSec = ps3TbHz();
  uint64_t tbDelay = usec / 1000000 * tbPerSec +
    usec % 1000000 * tbPerSec / 1000000;
  ps3ThreadSleepUntil(tbDelay + ps3TbRead());
  return 0;
}

