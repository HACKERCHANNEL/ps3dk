/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * mutex.c - Thread mutual exclusion
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3thread.h>
#include <ps3hv.h>

void ps3MutexLockF(ps3Mutex_t *mutex)
{
  struct ps3Thread *me = ps3ThreadSelf();
  if(mutex->owner == me)
    mutex->wo.token ++;
  else {
    ps3ThreadWaitAndAcquireWaitObj(&mutex->wo, 1);
    mutex->owner = me;
  }
}

int ps3MutexLockBeforeF(ps3Mutex_t *mutex, uint64_t tbval)
{
  int rc = 0;
  struct ps3Thread *me = ps3ThreadSelf();
  if(mutex->owner == me)
    mutex->wo.token ++;
  else {
    if(ps3ThreadWaitAndAcquireWaitObjTimeout(&mutex->wo, 1, tbval))
      mutex->owner = me;
    else
      rc = ETIME;
  }
  return rc;
}

void ps3MutexUnlockF(ps3Mutex_t *mutex)
{
  atomic_t v;
  if((v = mutex->wo.token - 1)) {
    mutex->wo.token = v;
    return;
  }
  mutex->owner = 0;
  __asm__ __volatile__ ("eieio");
  ps3ThreadSignalAndReleaseWaitObj(&mutex->wo);
}

