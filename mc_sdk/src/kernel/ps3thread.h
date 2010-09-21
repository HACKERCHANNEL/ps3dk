/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3thread.h - Prototypes for multithreading functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3THREAD_H__
#define __PS3THREAD_H__ 1

#include <sys/types.h>
#include <ps3types.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ps3Thread *ps3Thread_t; /* Opaque handle */

struct ps3WaitQueue {
  ps3Thread_t first, last;
};

struct ps3WaitObject {
  atomic_t token;
  struct ps3WaitQueue queue;
};

typedef struct ps3Mutex {
  struct ps3WaitObject wo;
  ps3Thread_t owner;
} ps3Mutex_t;

#define PS3_MUTEX_INITIALIZER {{0,{0,0}},0}

extern void ps3MutexLockF(ps3Mutex_t *mutex);
extern int ps3MutexLockBeforeF(ps3Mutex_t *mutex, uint64_t tbval);
extern void ps3MutexUnlockF(ps3Mutex_t *mutex);

static __inline__ ps3Thread_t ps3ThreadSelf(void)
{
  ps3Thread_t self;
  __asm__ ("mfsprg %0,3" : "=r"(self));
  return self;
}

static __inline__ atomic_t ps3QuickLock(atomic_t *lock, atomic_t token)
{
  atomic_t v;
  __asm__ __volatile__("1: lwarx %0,0,%1;"
		       "cmpwi %0,0;"
		       "bne- 1f;"
		       "stwcx. %2,0,%1;"
		       "bne- 1b;"
		       "isync;"
		       "1:" :
		       "=&r"(v) : "r"(lock),"r"(token):"cc","memory");
  return v;
}

static __inline__ int ps3MutexTryLock(ps3Mutex_t *mutex)
{
  atomic_t v;
  if(!(v = ps3QuickLock(&mutex->wo.token, 1))) {
    /* Mutex was unlocked */
    mutex->owner = ps3ThreadSelf();
  } else if(mutex->owner == ps3ThreadSelf()) {
    /* Locked by us */
    mutex->wo.token = v+1;
  } else
    return EBUSY;

  return 0;
}

static __inline__ int ps3MutexLock(ps3Mutex_t *mutex)
{
  if(ps3MutexTryLock(mutex))
    ps3MutexLockF(mutex);
  return 0;
}

static __inline__ int ps3MutexLockBefore(ps3Mutex_t *mutex, uint64_t tbval)
{
  if(ps3MutexTryLock(mutex))
    return ps3MutexLockBeforeF(mutex, tbval);
  else
    return 0;
}

static __inline__ void ps3MutexUnlock(ps3Mutex_t *mutex)
{
  atomic_t v;
  if(!(v = mutex->wo.token - 1)) {
    /* Really unlock */
    ps3MutexUnlockF(mutex);
  } else
    mutex->wo.token = v;
}

extern void ps3ThreadSleepUntil(uint64_t tbval);
extern void ps3ThreadSleepUntilInterruptable(uint64_t tbval);
extern void ps3ThreadYield(void);

extern void ps3ThreadSleep(uint64_t tbdelta);
extern void ps3ThreadSleepSeconds(uint64_t seconds);
extern void ps3ThreadSleepMillis(uint64_t millis);
extern void ps3ThreadSleepMicros(uint64_t micros);

extern ps3Thread_t ps3ThreadCreate(void (*func)(void *arg), void *arg,
				   uint64_t stack_size, uint8_t prio);

#ifdef __cplusplus
}
#endif

#endif
