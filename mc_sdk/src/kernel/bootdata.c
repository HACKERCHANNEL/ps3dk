/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * bootdata.c - Boot data management
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <string.h>
#include <ps3bootdata.h>
#include <ps3hv.h>

static uint64_t bootData[0x400/sizeof(uint64_t)];
static uint32_t bootDataSize;

static void getBootData(void) __attribute__ ((constructor));

static void getBootData(void)
{
  uint64_t lpar_id, boot_data_addr, boot_data_size, v2;
  void *boot_data;

  if(lv1_get_logical_partition_id(&lpar_id))
    return;

  if(lv1_get_repository_node_value(lpar_id,
				   0x0000000062690000, /* "bi" */
				   0x626f6f745f646174, /* "boot_dat" */
				   0x6164647265737300, /* "address" */
				   0, &boot_data_addr, &v2))
    return;
  
  if(lv1_get_repository_node_value(lpar_id,
				   0x0000000062690000, /* "bi" */
				   0x626f6f745f646174, /* "boot_dat" */
				   0x73697a6500000000, /* "size" */
				   0, &boot_data_size, &v2))
    return;

  if (boot_data_size < 0x400)
    return;

  /* MMU is not enabled at this point... */
  boot_data = (void *)boot_data_addr;

  memcpy(bootData, boot_data, 0x400);

  if (memcmp((char *)bootData, "cell_ext_os_area\0\0\0\x01", 0x14))
    return;

  bootDataSize = 0x400;
}

const ps3BootParams_t *ps3BootParameters(void)
{
  if(bootDataSize >= 0x200+sizeof(ps3BootParams_t))
    return (ps3BootParams_t *)&bootData[0x200/sizeof(uint64_t)];
  else
    return NULL;
}
