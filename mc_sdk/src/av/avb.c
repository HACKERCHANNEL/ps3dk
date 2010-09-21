/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * init.c - AVB functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3vuart.h>
#include <ps3av.h>

#include "avInternal.h"

static const struct {
  uint8_t vmode;
  uint8_t aspect;
  uint16_t width;
  uint16_t height;
} modeTab[] = {
  [PS3AV_MODE_480I] =       {  1, PS3AV_ASPECT_4_3 ,  720,  480 },
  [PS3AV_MODE_480P] =       {  5, PS3AV_ASPECT_4_3 ,  720,  480 },
  [PS3AV_MODE_720P_60HZ] =  {  9, PS3AV_ASPECT_4_3 , 1280,  720 },
  [PS3AV_MODE_1080I_60HZ] = {  7, PS3AV_ASPECT_16_9, 1920, 1080 },
  [PS3AV_MODE_1080P_60HZ] = { 11, PS3AV_ASPECT_16_9, 1920, 1080 },
  [PS3AV_MODE_576I] =       {  3, PS3AV_ASPECT_4_3 ,  720,  576 },
  [PS3AV_MODE_576P] =       {  6, PS3AV_ASPECT_4_3 ,  720,  576 },
  [PS3AV_MODE_720P_50HZ] =  { 10, PS3AV_ASPECT_4_3 , 1280,  720 },
  [PS3AV_MODE_1080I_50HZ] = {  8, PS3AV_ASPECT_16_9, 1920, 1080 },
  [PS3AV_MODE_1080P_50HZ] = { 12, PS3AV_ASPECT_16_9, 1920, 1080 },
  [PS3AV_MODE_WXGA] =       { 13, PS3AV_ASPECT_16_9, 1280,  768 },
  [PS3AV_MODE_SXGA] =       { 14, PS3AV_ASPECT_4_3 , 1280, 1024 },
  [PS3AV_MODE_WUXGA] =      { 15, PS3AV_ASPECT_16_9, 1920, 1200 }
};

int ps3AvAvbParamDualVideo(uint32_t hdmiMode, uint32_t avMultiMode)
{
  SEND_AV_CMD(avCmdAvbParamDualVideo, AV_CMD_AVB_PARAM,
	      .num_v_pkts = 2,
	      .num_a_pkts = 0,
	      .num_av_v_pkts = 2,
	      .num_av_a_pkts = 0,
	      .video_pkt = {
		[0] = { AV_CMD(avCmdVideoMode, AV_CMD_VIDEO_MODE,
			       .head = PS3AV_HEAD_A,
			       .mode = modeTab[hdmiMode].vmode,
			       .width = modeTab[hdmiMode].width,
			       .height = modeTab[hdmiMode].height,
			       .pitch = modeTab[hdmiMode].width*4,
			       .out_format = PS3AV_OUT_FORMAT_RGB12,
			       .video_format = PS3AV_VIDEO_FORMAT_ARGB8,
			       .rgb_order = PS3AV_RGB_ORDER_RGB) },
		[1] = { AV_CMD(avCmdVideoMode, AV_CMD_VIDEO_MODE,
			       .head = PS3AV_HEAD_B,
			       .mode = (avMultiMode == PS3AV_MODE_480I?
					16 : modeTab[avMultiMode].vmode),
			       .width = modeTab[avMultiMode].width,
			       .height = modeTab[avMultiMode].height,
			       .pitch = modeTab[avMultiMode].width*4,
			       .out_format = PS3AV_OUT_FORMAT_RGB12,
			       .video_format = PS3AV_VIDEO_FORMAT_ARGB8,
			       .rgb_order = PS3AV_RGB_ORDER_RGB) },
	      },
	      .av_video_pkt = {
		[0] = { AV_CMD(avCmdAvVideoCs, AV_CMD_AV_VIDEO_CS,
			       .avport = PS3AV_AVPORT_HDMI_0,
			       .mode = hdmiMode,
			       .csout = PS3AV_CS_YUV444,
			       .csin = PS3AV_CS_RGB,
			       .dither = PS3AV_DITHER_OFF,
			       .bitlen = PS3AV_BITLEN_8,
			       .superwhite = PS3AV_SUPERWHITE_OFF,
			       .aspect = modeTab[hdmiMode].aspect) },
		[1] = { AV_CMD(avCmdAvVideoCs, AV_CMD_AV_VIDEO_CS,
			       .avport = PS3AV_AVPORT_AVMULTI_0,
			       .mode = avMultiMode,
			       .csout = PS3AV_CS_YUV444,
			       .csin = PS3AV_CS_RGB,
			       .dither = PS3AV_DITHER_OFF,
			       .bitlen = PS3AV_BITLEN_8,
			       .superwhite = PS3AV_SUPERWHITE_OFF,
			       .aspect = modeTab[avMultiMode].aspect) }
	      });
}
