/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * init.c - GPU init functions
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <ps3hv.h>
#include <ps3gpu.h>
#include <ps3mmu.h>

#define DDR_SIZE (252*1024*1024)

#define L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_MODE_SET    0x100
#define L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_SYNC        0x101
#define L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_FLIP        0x102

#define L1GPU_DISPLAY_SYNC_HSYNC                    1
#define L1GPU_DISPLAY_SYNC_VSYNC                    2

static gpu_memory_handle_t memory_handle;
static lpar_addr_t ddr_lpar;
static void *ddr_va = NULL;

extern int ps3GpuInit(void)
{
  long status;

  status = lv1_gpu_open(0);
  status = lv1_gpu_context_attribute(0x0, 
				     L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_MODE_SET,
				     0, 0, 0, 0);	// head a
  status = lv1_gpu_context_attribute(0x0, 
				     L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_MODE_SET,
				     0, 0, 1, 0);	// head b
  status = lv1_gpu_context_attribute(0x0, 
				     L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_SYNC,
				     0, L1GPU_DISPLAY_SYNC_VSYNC, 0, 0);
  status = lv1_gpu_context_attribute(0x0, 
				     L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_SYNC,
				     1, L1GPU_DISPLAY_SYNC_VSYNC, 0, 0);

  status = lv1_gpu_memory_allocate(DDR_SIZE,
				   0, 0, 0, 0,
				   &memory_handle, &ddr_lpar);

  return 0;
}

void *ps3GpuGetVramBase(void)
{
  if(!ddr_va)
    ddr_va = ps3MmuIoRemap(ddr_lpar, 8*1024*1024);
  return ddr_va;
}

extern uint64_t ps3GpuGetVramSize(void)
{
  return DDR_SIZE;
}

