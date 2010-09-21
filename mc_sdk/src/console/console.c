/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * console.c - Frame buffer console
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3gpu.h>
#include <ps3console.h>
#include <ps3debug.h>
#include <ps3mmu.h>

static void *fb_addr;
static unsigned curs_row, curs_col;
static uint32_t curs_fg, curs_bg;

static void debugPutc(char c)
{
  ps3MmuOn();
  ps3ConsolePutChar(c);
}

void ps3ConsoleInit()
{
  fb_addr = ps3GpuGetVramBase();
  curs_row = curs_col = 4;
  curs_fg = 0xffffff;
  curs_bg = 0x000000;
  ps3DebugSetPutcFunc(debugPutc);
}

void ps3ConsoleCharAt(unsigned row, unsigned col, int c,
		      uint32_t fg, uint32_t bg)
{
  unsigned py = row * 16;
  unsigned px = col * 8;
  unsigned pitch = 720*4;
  const uint8_t *g = ps3ConsoleFont+c*16;

  uint32_t *fb = (uint32_t *)(fb_addr + px*4 + py * pitch);
  int i, j;
  for(i=0; i<16; i++) {
    uint8_t m = *g++;
    for(j=0; j<8; j++) {
      fb[j] = ((m&0x80)? fg : bg);
      m <<= 1;
    }
    fb += pitch/sizeof(uint32_t);
  }

}

void ps3ConsolePutChar(int c)
{
  switch(c) {
  case '\r':
    curs_col = 4;
    break;
  case '\n':
    curs_col = 4;
    curs_row++;
    break;
  default:
    ps3ConsoleCharAt(curs_row, curs_col++, c, curs_fg, curs_bg);
    break;
  }
}

void ps3ConsoleWrite(const char *txt, size_t len)
{
  while(len--)
    ps3ConsolePutChar(*txt++);
}

void ps3ConsoleWriteStr(const char *txt)
{
  while(*txt)
    ps3ConsolePutChar(*txt++);
}

