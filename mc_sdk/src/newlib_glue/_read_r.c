/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _read_r.c - File input
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>

_ssize_t _read_r(struct _reent *reent, int fd, void *buf, size_t count)
{
  reent->_errno = EBADF;
  return -1;
}
