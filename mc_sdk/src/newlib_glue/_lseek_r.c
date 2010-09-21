/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _lseek_r.c - File seek
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>

_off_t _lseek_r(struct _reent *reent, int fd, _off_t offset, int whence)
{
  reent->_errno = EBADF;
  return -1;
}
