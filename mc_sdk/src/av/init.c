/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * init.c - A/V init functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3vuart.h>
#include <ps3av.h>

#include "avInternal.h"

int ps3AvAvInit(uint32_t eventbit)
{
  SEND_AV_CMD(avCmdAvInit, AV_CMD_AV_INIT,
	      .eventbit = eventbit);
}

extern int ps3AvVideoInit(void)
{
  SEND_AV_CMD(avCmdGeneric, AV_CMD_VIDEO_INIT);
}

extern int ps3AvAudioInit(void)
{
  SEND_AV_CMD(avCmdGeneric, AV_CMD_AUDIO_INIT);
}



