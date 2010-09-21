/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _kill_r.c - Signal a process
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <reent.h>
#include <errno.h>
#include <signal.h>
#include <ps3hv.h>

int _kill_r(struct _reent *reent, int pid, int sig)
{
  if (pid != 0 && pid != -1) {
    /* No such process (group) */
    reent->_errno = ESRCH;
    return -1;
  }

  if (sig) {
    /* We're supposed to send a signal to ourselves. */
    /* If the signal is SIGCHLD, SIGURG, or SIGWINCH, the
       default behaviour is to ignore it.  Otherwise, we
       should stop somehow.  A panic should suffice. */
    if (sig != SIGCHLD && sig != SIGURG && sig != SIGWINCH)
      lv1_panic(1);
  }

  return 0;
}
