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
#include <ps3thread.h>


/* The newlib codebase contains 7 pre-initialized mutices.
   Among them is the malloc mutex.  Thus, we keep a pool
   of 8 mutices which can be allocated without using malloc.  */

#define NUM_PREALLOCATED_MUTICES 8


static ps3Mutex_t metaLock = PS3_MUTEX_INITIALIZER;

static ps3Mutex_t mutexPool[NUM_PREALLOCATED_MUTICES];
static int nextMutexIndex = 0;

void ps3NlGlueAutoLockAllocate(void **pMutex)
{
  ps3MutexLock(&metaLock);

  /* Need to check the magic again now that we have the lock... */
  if (*pMutex == __libc_autolock_magic) {

    if (nextMutexIndex >= NUM_PREALLOCATED_MUTICES) {
      /* Out of mutices!? */
      *pMutex = NULL;
    } else {
      *pMutex = &mutexPool[nextMutexIndex++];
    }

  }

  ps3MutexUnlock(&metaLock);
}

