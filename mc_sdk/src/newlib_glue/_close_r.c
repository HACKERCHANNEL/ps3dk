/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _close_r.c - File termination
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>

int _close_r(struct _reent *reent, int fd)
{
  reent->_errno = EBADF;
  return -1;
}
