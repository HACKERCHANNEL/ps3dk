/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * crt1.S - Startup code
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

/* SPRs */
#define SRR0	 0x01A	/* Save/Restore Register 0 */
#define SRR1	 0x01B	/* Save/Restore Register 1 */
#define CTRLF	 0x088
#define SPRG0    0x110
#define SPRG1    0x111
#define SPRG2    0x112
#define SPRG3    0x113

/* MSR bits */
#define MSR_SF	 (1<<63)        /* Enable 64 bit mode */
#define MSR_ISF	 (1<<61)        /* Interrupt 64b mode valid on 630 */
#define MSR_HV 	 (1<<60)        /* Hypervisor state */
#define MSR_VEC	 (1<<25)	/* Enable AltiVec */
#define MSR_POW	 (1<<18)	/* Enable Power Management */
#define MSR_WE	 (1<<18)	/* Wait State Enable */
#define MSR_TGPR (1<<17)	/* TLB Update registers in use */
#define MSR_CE	 (1<<17)	/* Critical Interrupt Enable */
#define MSR_ILE	 (1<<16)	/* Interrupt Little Endian */
#define MSR_EE	 (1<<15)	/* External Interrupt Enable */
#define MSR_PR	 (1<<14)	/* Problem State / Privilege Level */
#define MSR_FP	 (1<<13)	/* Floating Point enable */
#define MSR_ME	 (1<<12)	/* Machine Check Enable */
#define MSR_FE0	 (1<<11)	/* Floating Exception mode 0 */
#define MSR_SE	 (1<<10)	/* Single Step */
#define MSR_BE	 (1<<9)		/* Branch Trace */
#define MSR_DE	 (1<<9) 	/* Debug Exception Enable */
#define MSR_FE1	 (1<<8)		/* Floating Exception mode 1 */
#define MSR_IP	 (1<<6)		/* Exception prefix 0x000/0xFFF */
#define MSR_IR	 (1<<5) 	/* Instruction Relocate */
#define MSR_DR	 (1<<4) 	/* Data Relocate */
#define MSR_PE	 (1<<3)		/* Protection Enable */
#define MSR_PX	 (1<<2)		/* Protection Exclusive Mode */
#define MSR_PMM	 (1<<2)		/* Performance monitor */
#define MSR_RI	 (1<<1)		/* Recoverable Exception */
#define MSR_LE	 (1<<0) 	/* Little Endian */

