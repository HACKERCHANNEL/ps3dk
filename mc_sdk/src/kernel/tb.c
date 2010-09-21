/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * tb.c - Timebase
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3tb.h>
#include <ps3hv.h>
#include <ps3thread.h>

static uint64_t tbHz;

static void getTbHz(void) __attribute__ ((constructor));

static void getTbHz(void)
{
  int64_t r;
  uint64_t nodeid, v1, v2;

  r = lv1_get_repository_node_value(1,
				    0x0000000062650000,	/* "be" */
				    0x6265000000000000,	/* "be" */
				    0,
				    0,
				    &v1, &v2);
  nodeid = v1;
  
  r = lv1_get_repository_node_value(1,
				    0x0000000062650000,	/* "be" */
				    nodeid,
				    0x636c6f636b000000,	/* "clock" */
				    0,
				    &v1, &v2);
  tbHz = v1;
}

uint64_t ps3TbHz()
{
  return tbHz;
}
