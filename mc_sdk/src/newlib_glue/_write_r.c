/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _write_r.c - File output
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>

_ssize_t _write_r(struct _reent *reent, int fd, const void *buf, size_t count)
{
  if(fd == 1 || fd == 2) {
    ps3ConsoleWrite(buf, count);
    return count;
  }
  reent->_errno = EBADF;
  return -1;
}
