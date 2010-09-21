/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * write.c - VUART write function
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3hv.h>
#include <ps3vuart.h>
#include <ps3lpar.h>

ssize_t ps3VuartWrite(int unit, const unsigned char *data,
		      size_t length)
{
  int64_t status;
  uint64_t actual;
  status = lv1_write_virtual_uart(unit, ps3LparAddrFromPointer(data), length, &actual);
  if(status)
    return -1;
  else
    return actual;
}
