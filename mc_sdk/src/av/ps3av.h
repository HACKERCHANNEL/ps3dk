/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3av.h - Prototypes for the A/V library
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3AV_H__
#define __PS3AV_H__ 1

#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PS3AV_HEAD_A            0
#define PS3AV_HEAD_B            1

#define PS3AV_MODE_480I         0
#define PS3AV_MODE_480P         1
#define PS3AV_MODE_720P_60HZ    2
#define PS3AV_MODE_1080I_60HZ   3
#define PS3AV_MODE_1080P_60HZ   4
#define PS3AV_MODE_576I         5
#define PS3AV_MODE_576P         6
#define PS3AV_MODE_720P_50HZ    7
#define PS3AV_MODE_1080I_50HZ   8
#define PS3AV_MODE_1080P_50HZ   9
#define PS3AV_MODE_WXGA         10
#define PS3AV_MODE_SXGA         11
#define PS3AV_MODE_WUXGA        12

#define PS3AV_OUT_FORMAT_RGB12  0

#define PS3AV_VIDEO_FORMAT_NULL  0
#define PS3AV_VIDEO_FORMAT_ARGB8 7

#define PS3AV_RGB_ORDER_RGB     0
#define PS3AV_RGB_ORDER_BGR     1

#define PS3AV_AVPORT_HDMI_0     0x00
#define PS3AV_AVPORT_HDMI_1     0x01
#define PS3AV_AVPORT_AVMULTI_0  0x10
#define PS3AV_AVPORT_SPDIF_0    0x20
#define PS3AV_AVPORT_SPDIF_1    0x21

#define PS3AV_CS_RGB            0
#define PS3AV_CS_YUV444         1
#define PS3AV_CS_YUV422         2
#define PS3AV_CS_XVYCC          3

#define PS3AV_DITHER_OFF        0
#define PS3AV_DITHER_ON         1
#define PS3AV_DITHER_LEN(X)     ((X)<<1)

#define PS3AV_BITLEN_8          0
#define PS3AV_BITLEN_10         1
#define PS3AV_BITLEN_12         2

#define PS3AV_SUPERWHITE_OFF    0
#define PS3AV_SUPERWHITE_ON     1

#define PS3AV_ASPECT_16_9       0
#define PS3AV_ASPECT_4_3        1


extern int ps3AvAvInit(uint32_t eventbit);

extern int ps3AvVideoInit(void);

extern int ps3AvAudioInit(void);

extern int ps3AvAvbParamDualVideo(uint32_t hdmiMode, uint32_t avMultiMode);

#ifdef __cplusplus
}
#endif

#endif
