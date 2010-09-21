/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _sbrk_r.c - Heap management
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>
#include <ps3thread.h>
#include <ps3realmode.h>

extern char _start[], _end[];
static void *_current_break = &_end;

static ps3Mutex_t break_lock = PS3_MUTEX_INITIALIZER;

#define MAX_MEM (((char *)_start)+ps3RealMemSize())

void *_sbrk_r(struct _reent *reent, ptrdiff_t increment)
{
  ps3MutexLock(&break_lock);

  void *old_break = _current_break;
  if((increment > 0 && increment > MAX_MEM-(char *)_current_break) ||
     (increment < 0 && (~increment) >= ((char *)_current_break)-(char *)&_end)) {
    reent->_errno = ENOMEM;
    old_break = (void *)-1;
  } else
    _current_break = increment + (char *)_current_break;

  ps3MutexUnlock(&break_lock);

  return old_break;
}
