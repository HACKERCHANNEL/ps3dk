/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _getpid_r.c - Get process ID
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>

int _getpid_r(struct _reent *reent)
{
  /* There are no processes, so always return "0" */
  return 0;
}
