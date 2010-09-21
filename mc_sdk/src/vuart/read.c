/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * read.c - VUART read function
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3hv.h>
#include <ps3vuart.h>
#include <ps3thread.h>
#include <ps3lpar.h>

ssize_t ps3VuartRead(int unit, unsigned char *data,
		     size_t length, unsigned timeout)
{
  int64_t status;
  uint64_t actual;
  size_t total=0;
  for(;;) {
    status = lv1_read_virtual_uart(unit, ps3LparAddrFromPointer(data), length, &actual);
    if(status)
      return -1;
    total += actual;
    if(actual >= length)
      break;
    data += actual;
    length -= actual;
    if(!timeout) {
      break;
    } else if(timeout > 25000) {
      ps3ThreadSleepMicros(25000);
      timeout -= 25000;
    } else {
      ps3ThreadSleepMicros(timeout);
      timeout = 0;
    }
  }
  return total;
}
