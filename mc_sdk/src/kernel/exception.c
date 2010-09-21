/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * exception.c - Exception handling
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <stdio.h>
#include <ps3types.h>

void ps3ExceptionEntry(uint64_t vector, uint64_t *regs)
{
  uint64_t srr0;
  __asm__("mfspr %0,26" : "=r"(srr0));
  ps3DebugPrintf("Exception %x occurred at %x\n", vector, srr0);
  for(;;)
    ;
}
