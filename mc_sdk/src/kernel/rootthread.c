/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * rootthread.c - The thread structure for the root thread
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3thread.h>

#include "threadInternal.h"

struct ps3Thread ps3RootThread = {
  .state = RUNNING,
  .priority = 100
};

