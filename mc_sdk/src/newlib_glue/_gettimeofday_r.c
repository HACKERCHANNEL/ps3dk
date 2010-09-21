/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _gettimeofday_r.c - Realtime clock
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>
#include <sys/time.h>

#include <ps3hv.h>
#include <ps3bootdata.h>
#include <ps3tb.h>

static uint64_t epochRtc, epochTb;

static void getEpoch(void) __attribute__ ((constructor));

static void getEpoch(void)
{
  lv1_get_rtc(&epochRtc, &epochTb);
}

int _gettimeofday_r(struct _reent *reent, struct timeval *tp, struct timezone *tzp)
{
  if (tzp != NULL) {
    /* Not supported... */
    reent->_errno = EINVAL;
    return -1;
  }
  if (tp != NULL) {
    uint64_t tbhz = ps3TbHz();
    uint64_t tbdiff = ps3TbRead() - epochTb;
    uint64_t rtc = epochRtc + (tbdiff / tbhz);
    const ps3BootParams_t *bp = ps3BootParameters();
    int64_t offs = (bp && bp->numParams>=1? bp->rtcDiff : 946684800);
    tp->tv_sec = rtc + offs;
    tp->tv_usec = tbdiff % tbhz * 1000000 / tbhz;
  }
  return 0;
}

