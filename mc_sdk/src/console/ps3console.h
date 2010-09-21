/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ps3console.h - Prototypes for console functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */
#ifndef __PS3CONSOLE_H__
#define __PS3CONSOLE_H__ 1

#include <sys/types.h>
#include <ps3types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t ps3ConsoleFont[];

extern void ps3ConsoleInit(void);
extern void ps3ConsoleCharAt(unsigned row, unsigned col, int c, uint32_t fg, uint32_t bg);
extern void ps3ConsolePutChar(int c);
extern void ps3ConsoleWrite(const char *txt, size_t len);
extern void ps3ConsoleWriteStr(const char *txt);

#ifdef __cplusplus
}
#endif

#endif
