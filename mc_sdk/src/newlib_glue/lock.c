/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * lock.c - Mutex locks
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/lock.h>
#include <stdlib.h>
#include <ps3thread.h>

/* In autolock.c */
extern void ps3NlGlueAutoLockAllocate(void **pMutex);

int __libc_lock_init(__libc_lock_t *lock)
{
  lock->lock = calloc(1, sizeof(ps3Mutex_t));
  return 0;
}

int __libc_lock_close(__libc_lock_t *lock)
{
  if (lock->lock) {
    free(lock->lock);
    lock->lock = NULL;
  }
  return 0;
}

int __libc_lock_acquire(__libc_lock_t *lock)
{
  if (lock->lock == __libc_autolock_magic)
    ps3NlGlueAutoLockAllocate(&lock->lock);
  ps3MutexLock((ps3Mutex_t *)lock->lock);
  return 0;
}

int __libc_lock_try_acquire(__libc_lock_t *lock)
{
  if (lock->lock == __libc_autolock_magic)
    ps3NlGlueAutoLockAllocate(&lock->lock);
  ps3MutexTryLock((ps3Mutex_t *)lock->lock);
  return 0;
}

int __libc_lock_release(__libc_lock_t *lock)
{
  if (lock->lock == __libc_autolock_magic)
    ps3NlGlueAutoLockAllocate(&lock->lock);
  ps3MutexUnlock((ps3Mutex_t *)lock->lock);
  return 0;
}

