/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __AVINTERNAL_H__
#define __AVINTERNAL_H__ 1

#include <sys/types.h>

#define AV_CMD_AV_INIT     0x00000001
#define AV_CMD_AV_VIDEO_CS 0x00010001

#define AV_CMD_VIDEO_INIT  0x01000001
#define AV_CMD_VIDEO_MODE  0x01000002

#define AV_CMD_AUDIO_INIT  0x02000001

#define AV_CMD_AVB_PARAM   0x04000001

#define AV_HEADER_VERSION  0x0205

struct avCmdHeader {
  uint16_t version;
  uint16_t length;
  uint32_t cmdcode;
};

struct avCmdGeneric {
  struct avCmdHeader header;
};

struct avCmdAvInit {
  struct avCmdHeader header;
  uint32_t eventbit;
};

struct avCmdVideoMode {
  struct avCmdHeader header;
  uint32_t head;
  uint32_t reserved1;
  uint32_t mode;
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint32_t out_format;
  uint32_t video_format;
  uint32_t rgb_order;
  uint32_t reserved2;
};

struct avCmdAvVideoCs {
  struct avCmdHeader header;
  uint16_t avport;
  uint16_t mode;
  uint16_t csout;
  uint16_t csin;
  uint8_t dither;
  uint8_t bitlen;
  uint8_t superwhite;
  uint8_t aspect;
};

struct avCmdAvbParamDualVideo {
  struct avCmdHeader header;
  uint16_t num_v_pkts;
  uint16_t num_a_pkts;
  uint16_t num_av_v_pkts;
  uint16_t num_av_a_pkts;
  struct avCmdVideoMode video_pkt[2];
  struct avCmdAvVideoCs av_video_pkt[2];
};

#define AV_CMD(type_, code_, ...)					\
    .header = {								\
      .version = AV_HEADER_VERSION,					\
      .length = sizeof(struct type_) - sizeof(struct avCmdHeader)	\
        + sizeof(uint32_t),						\
      .cmdcode = code_							\
    },									\
    __VA_ARGS__

#define SEND_AV_CMD(type_, code_, ...)				\
  do {								\
    struct type_ type_ = {					\
      AV_CMD(type_, code_, __VA_ARGS__)				\
    };								\
    return avCommand((const uint8_t *)&type_, sizeof(type_));	\
  } while(0)

extern int avCommand(const uint8_t *buf, size_t len);

#endif
