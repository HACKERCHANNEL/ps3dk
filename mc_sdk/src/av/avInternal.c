/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3vuart.h>
#include <ps3av.h>

#include "avInternal.h"

/* Hum hum...
   In some cases the av vuart doesn't seem to reply,
   and a long timeout prevents the program from starting
   at all.  We don't really need to see the response,
   so don't bother waiting for it... */

/* #define TIMEOUT 5000000 */
#define TIMEOUT 0

int avCommand(const uint8_t *cmd, size_t cmdlen)
{
  static unsigned char buf[1024];
  ssize_t actual;
  int i;

  ps3VuartWrite(PS3_VUART_UNIT_AV, cmd, cmdlen);

  actual = ps3VuartRead(PS3_VUART_UNIT_AV, buf, 4, TIMEOUT);

  if(actual < 0)
    return actual;

  if(actual == 4) {
    int extra_size = (buf[2]<<8)|buf[3];
    actual = ps3VuartRead(PS3_VUART_UNIT_AV, buf+4, extra_size, TIMEOUT);
    if(actual >= 0)
      actual += 4;
  }

  return actual;
}
