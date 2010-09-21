/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * mmu.c - Memory management
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <sys/types.h>
#include <stdbool.h>
#include <ps3hv.h>
#include <ps3lpar.h>
#include <ps3realmode.h>
#include <ps3mmu.h>
#include <spr.h>

/* First word of PTE */
#define PTE_V    0x01   /* Valid */
#define PTE_H    0x02   /* Hash function selector */
#define PTE_L    0x04   /* Large page */
#define PTE_SW   0x78   /* Reserved for software use */
#define PTE_AVPN 0xffffffffffffff80  /* Abbreviated virtual page number */

/* Second work of PTE */
#define PTE_PP   0x03   /* Page protection */
#define PTE_N    0x04   /* No execute */
#define PTE_G    0x08   /* Guarded */
#define PTE_M    0x10   /* Memory coherency */
#define PTE_I    0x20   /* Cache inhibit */
#define PTE_W    0x40   /* Write-through */
#define PTE_C    0x80   /* Changed */
#define PTE_R    0x100  /* Referenced */
#define PTE_AC   0x200  /* Address compare */
#define PTE_RPN  0x3ffffffffffff000  /* Physical page number */


static unsigned long as_id;
static unsigned long htab_hash_mask;

static int allocated_4k_pages = 0;


/* The method used here to manage the hash table is rather
   simplistic:  We just add entries to the PTEGs and never
   remove them.  Because of this, the only housekeeping state
   needed is to keep track of how many slots of each PTEG has
   been used so far. */

/* We request at least 2048 PTEG:s (256K htab).  If we get more, some
   PTEGs will be aliased in this table, but that doesn't matter. */
static char hash_table_fill[2048];


static void map_page(unsigned long vaddr, unsigned long paddr, int shift,
		     unsigned long vflags, unsigned long pflags)
{
  int pteg, slot;
  long status;
  unsigned long v, r;

  /* Hash the virtual address.  (Only the primary hash function is used.) */
  pteg = (((vaddr >> 28) & 0x7fffffffffUL) ^
	  ((vaddr & 0x0fffffffUL) >> shift))
    & htab_hash_mask;

  v = ((vaddr >> 23)<<7) | vflags;
  r = (paddr & ~((1 << shift)-1)) | pflags;

  slot = hash_table_fill[pteg&(sizeof(hash_table_fill)-1)]++;
  if(slot >= 8) {
    return;
  }

  /* The hardware will check even numbered slots before odd
     numbered, modify slot number to fill the even ones first */
  slot = ((slot<<1)&6)|((slot>>2)&1);

  slot += pteg * 8;

  status = lv1_write_htab_entry(as_id, slot, v, r);
}

static unsigned long alloc_4k_pages(long cnt)
{
  long p;
  /* We only have one SLB entry setup for 4k pages, so no
     more than 65536 pages of vmem allowed in this region */
  if(allocated_4k_pages + cnt > 65536) {
    p = 0;
  } else {
    p = allocated_4k_pages;
    allocated_4k_pages += cnt;
    allocated_4k_pages += 17;  /* Red zone to help find bugs... */
  }
  return 0xf000000000000000+(p<<12);
}

static __inline__ bool dataRelocationIsOn(void)
{
  uint64_t msr;
  __asm__("mfmsr %0" : "=r"(msr));
  return !!(msr & MSR_DR);
}

void *ps3MmuIoRemap(lpar_addr_t paddr, uint64_t size)
{
  long i;
  long cnt = (size + 4095)>>12;
  unsigned long vaddr = alloc_4k_pages(cnt);
  for(i=0; i<cnt; i++)
    map_page(vaddr+(i<<12), paddr+(i<<12), 12, PTE_V, PTE_I|PTE_M|PTE_G|PTE_N);
  return (void*)vaddr;
}


void ps3MmuSetupSlb()
{
  /* Define a couple of SLB entries.
     For simplicity, VSID == ESID is used. */

  asm volatile("isync":::"memory");

  /* SLB0 is our main image, 0 and up mapped to the start of RAM
     (lpar 0).  Large pages (16MB) here. */
  asm volatile("slbmte  %0,%1"::"r" (0x0000d00000000500), "r" (0x8000000008000000) : "memory");

  /* Remove any old SLB entries (except 0, which we just set) */
  asm volatile("isync; slbia; isync":::"memory");

  /* 0xf000000000000000 is for small page (4K) allocation to
     whereever is needed. */
  asm volatile("slbmte  %0,%1":: "r" (0x0000f00000000400), "r" (0xf000000008000001) : "memory" );
}

lpar_addr_t ps3MmuVirtToLpar(const void *addr)
{
  unsigned long vaddr = (unsigned long)addr;
  /* FIXME */
  if (vaddr >= 0x8000000000000000 &&
      vaddr < 0x8000000000000000 + ps3RealMemSize())
    return vaddr - 0x8000000000000000;
  else
    return PS3_INVALID_LPAR_ADDR;
}

extern void *ps3MmuLparToVirt(lpar_addr_t addr)
{
  /* FIXME */
  if (addr < ps3RealMemSize())
    return (void *)(0x8000000000000000 | addr);
  else
    return NULL;
}

lpar_addr_t ps3LparAddrFromPointer(const void *addr)
{
  return dataRelocationIsOn()? ps3MmuVirtToLpar(addr) : ps3RealToLpar(addr);
}

void *ps3LparAddrToPointer(lpar_addr_t addr)
{
  return dataRelocationIsOn()? ps3MmuLparToVirt(addr) : ps3RealFromLpar(addr);
}

void ps3MmuInit(void)
{
  long r;
  int i;
  unsigned long act_size, addr, rm_size;
  unsigned long page_sizes, number_of_page_sizes, htab_size;

  /* Check size of "real memory" */
  rm_size = ps3RealMemSize();

  ps3MmuSetupSlb();
 
  /*
   * unsigned char[4] page_sizes contains log2(page sizes):
   *       16(64K), 20(1M), 24(16M)
   */
  page_sizes           = (24UL << 56) | (16UL << 48);
  number_of_page_sizes = 2;	/* maximum value for BPA is 2 */

  htab_size        = 18; /* 256KB */

  r = lv1_construct_virtual_address_space(htab_size, number_of_page_sizes,
					  page_sizes, &as_id, &act_size);

  htab_hash_mask = (act_size >> 7) - 1;

  /* Create identify mapping for real memory */
  for(addr=0; addr<rm_size; addr+=(1<<24))
    map_page(0xd000000000000000+addr, addr, 24, PTE_V|PTE_L, PTE_M);

  r = lv1_select_virtual_address_space(as_id);
}

void ps3MmuTerm(void)
{
  unsigned long puid;
  ps3MmuOff();
  lv1_select_virtual_address_space(0);
  lv1_destruct_virtual_address_space(as_id);
  lv1_get_logical_pu_id(&puid);
  lv1_configure_irq_state_bitmap(puid, 0, 0);
}
