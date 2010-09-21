/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * scheduler.c - Thread scheduling
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3thread.h>
#include <ps3tb.h>

#include "threadInternal.h"

static atomic_t schedulerLock = 0;

#define SCHEDULER_LOCK() do { } while(ps3QuickLock(&schedulerLock, 1))
#define SCHEDULER_UNLOCK() do { __asm__ __volatile__("isync"); schedulerLock=0; } while(0)

static struct ps3Thread *readyQueue = NULL;
static struct ps3Thread *timeoutQueue = NULL;

static void addWaitQueue(struct ps3Thread *t, struct ps3WaitQueue *q)
{
  t->next = NULL;
  if (q->first == NULL)
    q->first = q->last = t;
  else
    q->last = q->last->next = t; 
}

static struct ps3Thread *popWaitQueue(struct ps3WaitQueue *q)
{
  struct ps3Thread *t;
  if ((t = q->first) != NULL) {
    if ((q->first = t->next) == NULL)
      q->last = NULL;
  }
  return t;
}

static void removeWaitQueue(struct ps3Thread *t, struct ps3WaitQueue *q)
{
  if (t == q->first) {
    if ((q->first = t->next) == NULL)
      q->last = NULL;
  } else {
    struct ps3Thread *prev, *link;
    for (prev = q->first; prev; prev = link)
      if ((link = prev->next) == t) {
	if ((prev->next = t->next) == NULL)
	  q->last = prev;
	break;
      }
  }
}

static void addReady(struct ps3Thread *t)
{
  uint8_t p = t->priority;
  struct ps3Thread **link = &readyQueue;
  while (*link != NULL) {
    if ((*link)->priority < p)
      break;
    link = &(*link)->next;
  }
  t->next = *link;
  *link = t;
}

static void addTimeout(struct ps3Thread *t)
{
  uint64_t to = t->timeout;
  struct ps3Thread **link = &timeoutQueue;
  while (*link != NULL) {
    if ((int64_t)(to - (*link)->timeout) < 0)
      break;
    link = &(*link)->next_timeout;
  }
  t->next_timeout = *link;
  *link = t;
}

static void removeTimeout(struct ps3Thread *t)
{
  if (timeoutQueue == t)
    timeoutQueue = t->next_timeout;
  else {
    struct ps3Thread *prev, *link;
    for (prev = timeoutQueue; prev; prev = link)
      if ((link = prev->next_timeout) == t) {
	prev->next_timeout = t->next_timeout;
	break;
      }
  }
}

static struct ps3Thread *findReadyThread(void)
{
  struct ps3Thread *t;
  while ((t = timeoutQueue) != NULL) {
    int64_t dly = (int64_t)(t->timeout - ps3TbRead());
    if (dly <= 0) {
      timeoutQueue = t->next_timeout;
      if (t->waitobject != NULL)
	removeWaitQueue(t, &t->waitobject->queue);
      t->state = READY;
      addReady(t);
    } else {
      if (dly > 0x7fffffff)
	dly = 0x7fffffff;
      __asm__ __volatile__("mtdec %0" : : "r"(dly));
      break;
    }
  }
  t = readyQueue;
  if (t != NULL) {
    readyQueue = t->next;
    t->state = RUNNING;
  }
  return t;
}

static void reschedule()
{
  ps3ThreadContextSwitch(findReadyThread());
}

struct ps3Thread *ps3ThreadIdleFunc(void)
{
  struct ps3Thread *t;

  do {
    SCHEDULER_UNLOCK();

    __asm__ __volatile__ ("or 1,1,1"); /* low prio */

    /* Note that readyQueue and timeoutQueue are accessed
       without the scheduler lock here.  This is ok,
       since they are only compared to NULL and not dereferenced,
       and the result is only used as trigger for reclaiming the
       lock.  Once the lock is obtained, the values are rechecked. */
    while (*(struct ps3Thread * volatile *)&readyQueue == NULL)
      if (*(struct ps3Thread * volatile *)&timeoutQueue != NULL) {
	uint64_t dec;
	__asm__ __volatile__("mfdec %0" : "=r" (dec));
	if ((int32_t)dec < 0)
	  break;
      }

    __asm__ __volatile__ ("or 2,2,2"); /* normal prio */

    SCHEDULER_LOCK();
    t = findReadyThread();
  } while(t == NULL);

  return t;
}

void ps3ThreadWrapper(void (*func)(void *arg), void *arg)
{
  SCHEDULER_UNLOCK();
  (*func)(arg);
  SCHEDULER_LOCK();
  struct ps3Thread *me = ps3ThreadSelf();
  me->state = DEAD;
  reschedule();
  /* Rescheduling from a dead thread should never return... */
  lv1_panic(0);
}

void ps3ThreadSuspendTimeout(uint64_t tbval)
{
  SCHEDULER_LOCK();
  struct ps3Thread *me = ps3ThreadSelf();
  me->timeout = tbval;
  me->state = WAIT_TIMEOUT;
  addTimeout(me);
  reschedule();
  SCHEDULER_UNLOCK();
}

void ps3ThreadSuspendNoTimeout(uint64_t tbval)
{
  SCHEDULER_LOCK();
  struct ps3Thread *me = ps3ThreadSelf();
  me->state = WAIT_NOTIMEOUT;
  reschedule();
  SCHEDULER_UNLOCK();
}

void ps3ThreadWaitAndAcquireWaitObj(struct ps3WaitObject *wo, atomic_t v)
{
  SCHEDULER_LOCK();
  if(!ps3QuickLock(&wo->token, v)) {
    SCHEDULER_UNLOCK();
    return;
  }
  struct ps3Thread *me = ps3ThreadSelf();
  me->waitobject = wo;
  me->waittoken = v;
  me->state = WAIT_NOTIMEOUT;
  addWaitQueue(me, &wo->queue);
  reschedule();
  SCHEDULER_UNLOCK();
}

int ps3ThreadWaitAndAcquireWaitObjTimeout(struct ps3WaitObject *wo, atomic_t v, uint64_t tbval)
{
  int result = 1;
  SCHEDULER_LOCK();
  if(!ps3QuickLock(&wo->token, v)) {
    SCHEDULER_UNLOCK();
    return 1;
  }
  struct ps3Thread *me = ps3ThreadSelf();
  me->waitobject = wo;
  me->waittoken = v;
  me->timeout = tbval;
  me->state = WAIT_TIMEOUT;
  addWaitQueue(me, &wo->queue);
  addTimeout(me);
  reschedule();
  if(me->waitobject != NULL) {
    /* Wait timed out */
    me->waitobject = NULL;
    result = 0;
  }
  SCHEDULER_UNLOCK();
  return result;
}

void ps3ThreadSignalAndReleaseWaitObj(struct ps3WaitObject *wo)
{
  SCHEDULER_LOCK();
  struct ps3Thread *t = popWaitQueue(&wo->queue);
  if (t == NULL)
    wo->token = 0;
  else {
    /* Threaded entry */
    struct ps3Thread *me = ps3ThreadSelf();
    if (t->state == WAIT_TIMEOUT)
      removeTimeout(t);
    wo->token = t->waittoken;
    t->waitobject = NULL;
    t->state = READY;
    addReady(t);
    if(t->priority > me->priority) {
      me->state = READY;
      addReady(me);
      reschedule();
    }
  }
  SCHEDULER_UNLOCK();
}

void ps3ThreadAdd(struct ps3Thread *t)
{
  SCHEDULER_LOCK();
  struct ps3Thread *me = ps3ThreadSelf();
  t->waitobject = NULL;
  t->state = READY;
  addReady(t);
  if(t->priority > me->priority) {
    me->state = READY;
    addReady(me);
    reschedule();
  }
  SCHEDULER_UNLOCK();
}

void ps3ThreadYield(void)
{
  SCHEDULER_LOCK();
  struct ps3Thread *me = ps3ThreadSelf();
  me->state = READY;
  addReady(me);
  reschedule();
  SCHEDULER_UNLOCK();
}

void ps3ThreadSlaveEntry(void)
{
  ps3MmuSetupSlb();
  SCHEDULER_LOCK();
  ps3ThreadContextInit();
  SCHEDULER_UNLOCK();
}

