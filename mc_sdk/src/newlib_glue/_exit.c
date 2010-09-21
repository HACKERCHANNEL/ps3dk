/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * _exit.c - Terminate the program
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <unistd.h>
#include <ps3hv.h>

void _exit(int status)
{
  /* exit the application by rebooting the system */
  lv1_panic(1);
}
