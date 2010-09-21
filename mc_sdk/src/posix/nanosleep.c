/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * nanosleep.c - POSIX compatibility function nanosleep()
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <time.h>
#include <ps3tb.h>
#include <ps3thread.h>

int nanosleep(const struct timespec *req, struct timespec *rem)
{
  const struct timespec reqt = *req;
  if (reqt.tv_sec < 0 ||
      reqt.tv_nsec < 0 ||
      reqt.tv_nsec >= 1000000000L) {
    errno = EINVAL;
    return -1;
  }
  uint64_t t1, t0 = ps3TbRead();
  uint64_t tbPerSec = ps3TbHz();
  uint64_t tbDelay = reqt.tv_sec * tbPerSec +
    reqt.tv_nsec * tbPerSec / 1000000000;
  ps3ThreadSleepUntilInterruptable(t0 + tbDelay);
  t1 = ps3TbRead();
  if ((uint64_t)(t1 - t0) >= tbDelay)
    return 0;
  else {
    if (rem != NULL) {
      tbDelay -= (uint64_t)(t1 - t0);
      rem->tv_sec = tbDelay / tbPerSec;
      rem->tv_nsec = tbDelay % tbPerSec * 1000000000 / tbPerSec;
    }
    errno = EINTR;
    return -1;
  }
}

