/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _fstat_r.c - File status
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>
#include <sys/stat.h>

int _fstat_r(struct _reent *reent, int fd, struct stat *buf)
{
  reent->_errno = EBADF;
  return -1;
}
