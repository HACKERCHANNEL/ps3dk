/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __THREADINTERNAL_H__
#define __THREADINTERNAL_H__ 1

#define REGISTER_SAVE_CNT (18+18+3)

typedef enum {
  RUNNING,
  READY,
  WAIT_TIMEOUT,
  WAIT_NOTIMEOUT,
  DEAD,
} ps3ThreadState_t;

struct ps3Thread {
  uint64_t reg_save[REGISTER_SAVE_CNT];
  ps3ThreadState_t state;
  struct ps3Thread *next, *next_timeout;
  struct ps3WaitObject *waitobject;
  atomic_t waittoken;
  uint64_t timeout;
  uint8_t priority;
};

extern void ps3ThreadContextSwitch(struct ps3Thread *newThread);
extern void ps3ThreadContextInit(void);
extern void ps3ThreadInit(struct ps3Thread *thread, void (*func)(void *arg), void *arg, void *stackPointer);
extern void ps3ThreadAdd(struct ps3Thread *thread);
extern void ps3ThreadSuspendTimeout(uint64_t tbval);
extern void ps3ThreadSuspendNoTimeout(uint64_t tbval);
extern void ps3ThreadWaitAndAcquireWaitObj(struct ps3WaitObject *wo, atomic_t v);
extern int ps3ThreadWaitAndAcquireWaitObjTimeout(struct ps3WaitObject *wo, atomic_t v, uint64_t tbval);
extern void ps3ThreadSignalAndReleaseWaitObj(struct ps3WaitObject *wo);

#endif
