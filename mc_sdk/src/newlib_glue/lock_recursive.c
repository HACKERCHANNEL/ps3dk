/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * lock_recursive.c - Recursive mutex locks
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/lock.h>
#include <stdlib.h>
#include <ps3thread.h>

/* In autolock.c */
extern void ps3NlGlueAutoLockAllocate(void **pMutex);

int __libc_lock_init_recursive(__libc_lock_recursive_t *lock)
{
  lock->lock = calloc(1, sizeof(ps3Mutex_t));
  return 0;
}

int __libc_lock_close_recursive(__libc_lock_recursive_t *lock)
{
  if (lock->lock) {
    free(lock->lock);
    lock->lock = NULL;
  }
  return 0;
}

int __libc_lock_acquire_recursive(__libc_lock_recursive_t *lock)
{
  if (lock->lock == __libc_autolock_magic)
    ps3NlGlueAutoLockAllocate(&lock->lock);
  ps3MutexLock((ps3Mutex_t *)lock->lock);
  return 0;
}

int __libc_lock_try_acquire_recursive(__libc_lock_recursive_t *lock)
{
  if (lock->lock == __libc_autolock_magic)
    ps3NlGlueAutoLockAllocate(&lock->lock);
  ps3MutexTryLock((ps3Mutex_t *)lock->lock);
  return 0;
}

int __libc_lock_release_recursive(__libc_lock_recursive_t *lock)
{
  if (lock->lock == __libc_autolock_magic)
    ps3NlGlueAutoLockAllocate(&lock->lock);
  ps3MutexUnlock((ps3Mutex_t *)lock->lock);
  return 0;
}

