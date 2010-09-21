/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * debug.c - Debugging
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <ps3debug.h>

#include <stdarg.h>

static char buffer[1024];
static unsigned bufHead, bufTail;

static void defaultPutcFunc(char c)
{
  buffer[bufTail] = c;
  if (++bufTail == sizeof(buffer))
    bufTail = 0;
  if (bufTail == bufHead)
    /* Buffer is full, discard oldest entry */
    if (++bufHead == sizeof(buffer))
      bufHead = 0;
}

static void (*putcFunc)(char) = defaultPutcFunc;

void ps3DebugPutc(char c)
{
  (*putcFunc)(c);
}

void ps3DebugPuts(const char *s)
{
  while(*s)
    ps3DebugPutc(*s++);
}

void ps3DebugPutHex(uint64_t n)
{
  int i;
  for (i=60; i>=0; i-=4)
    ps3DebugPutc("0123456789ABCDEF"[(n>>i)&0xf]);
}

void ps3DebugPutUnsignedDecimal(uint64_t n)
{
  char buf[20];
  int i;
  for (i=0; i<20; i++)
    if (n < 10)
      break;
    else {
      buf[i] = '0'+(n%10);
      n /= 10;
    }
  ps3DebugPutc('0'+n);
  while (i>0)
    ps3DebugPutc(buf[--i]);
}

void ps3DebugPutDecimal(int64_t n)
{
  if (n<0) {
    ps3DebugPutc('-');
    n = -n;
  }
  ps3DebugPutUnsignedDecimal((uint64_t)n);
}

void ps3DebugPrintf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  while(*fmt) {
    if(*fmt == '%')
    again:
      switch(*++fmt) {
      case 0:
	continue;
      case 'x':
	ps3DebugPutHex(va_arg(ap, uint64_t));
	break;
      case 'd':
	ps3DebugPutDecimal(va_arg(ap, int64_t));
	break;
      case 'u':
	ps3DebugPutUnsignedDecimal(va_arg(ap, uint64_t));
	break;
      case 's':
	ps3DebugPuts(va_arg(ap, const char *));
	break;
      case 'l':
	goto again;
      default:
	ps3DebugPutc(*fmt);
	break;
      }
    else
      ps3DebugPutc(*fmt);
    fmt++;
  }
  va_end(ap);
}

void ps3DebugSetPutcFunc(void (*func)(char c))
{
  if (func) {
    putcFunc = func;
    while (bufHead != bufTail) {
      (*func)(buffer[bufHead]);
      if (++bufHead == sizeof(buffer))
	bufHead = 0;
    }
  } else
    putcFunc = defaultPutcFunc;
}
