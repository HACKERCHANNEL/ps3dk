/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3bootdata.h - Prototypes for boot data functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3BOOTDATA_H__
#define __PS3BOOTDATA_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <ps3types.h>

#define PS3_AV_MULTI_OUT_NTSC60    0
#define PS3_AV_MULTI_OUT_PAL50_RGB 1
#define PS3_AV_MULTI_OUT_PAL50     2

typedef struct ps3BootParams {
  uint32_t bootFlag;
  uint32_t reserved1[3];
  uint32_t numParams;
  uint32_t reserved2[3];
  /* Parameter 0 */
  int64_t rtcDiff;
  uint8_t avMultiOut;
  uint8_t ctrlButton;
  uint8_t reserved3[6];
  /* Parameter 1 */
  uint32_t staticIpAddr;
  uint32_t netmask;
  uint32_t defaultRoute;
  uint32_t reserved4;
  /* Parameter 2 */
  uint32_t dnsIpAddr[2];
  uint32_t reserved5[2];
} ps3BootParams_t;

extern const ps3BootParams_t *ps3BootParameters(void);

#ifdef __cplusplus
}
#endif

#endif
