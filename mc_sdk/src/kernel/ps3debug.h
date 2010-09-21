/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3debug.h - Prototypes for debugging functions
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3DEBUG_H__
#define __PS3DEBUG_H__ 1

#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define PS3_DEBUG(...) ps3DebugPrintf(__VA_ARGS__)
#else
#define PS3_DEBUG(...) do {} while(0)
#endif

extern void ps3DebugPutc(char c);
extern void ps3DebugPuts(const char *s);
extern void ps3DebugPutHex(uint64_t n);
extern void ps3DebugPutUnsignedDecimal(uint64_t n);
extern void ps3DebugPutDecimal(int64_t n);
extern void ps3DebugPrintf(const char *fmt, ...) __attribute__((format (printf, 1, 2)));

extern void ps3DebugSetPutcFunc(void (*func)(char c));

#ifdef __cplusplus
}
#endif

#endif
