/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * crt1.S - Startup code
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include "spr.h"

	
#define LOAD_REG_IMMEDIATE(reg,expr)		\
	lis     reg,(expr)@highest;		\
	ori     reg,reg,(expr)@higher;		\
	rldicr  reg,reg,32,31;			\
	oris    reg,reg,(expr)@h;		\
	ori     reg,reg,(expr)@l;


	.global _start

	.section ".boot","ax"

	.balign 8

	/* This is the entry point of the main PPE thread
	   when we're started from kboot.

	   r3 contains a pointer to the "device tree"
	   r4 contains the address to which the program was loaded
	      (When started from kboot, the program isn't loaded to
	       address 0, but to an arbitrary address.  The first
	       256 bytes are copied to 0, but the rest needs to be
	       moved by the program itself)
	   r5 contains 0
	*/
_start:	
	b	relocate_and_start_master
	trap

	/* This is the entry point of the PPE slaves (of which
	   there is exactly one in the case of the PS3) when
	   we're started from kboot.

	   r3 holds the number of the CPU (= 1)   */
	
	. = _start + 0x60

secondary_hold:
	or r1,r1,r1 /* low prio */
	/* Wait until we have somewhere else to go... */
1:	ld r0, (secondary_dispatch-_start)(0)
	or. r0,r0,r0
	beq 1b
	mtctr r0
	bctr

	.align 3
secondary_dispatch:
	.llong 0

dispatch_slave:
	std r3, (secondary_dispatch-_start)(0)
	blr


	/* This is the entry point of both the main and slave
	   PPE threads when we're started from GameOS.

	   r3 appears to hold the CPU number, but we'll check nevertheless */

	. = _start + 0x100

	mfspr	r3, CTRLF
	cntlzw.	r3, r3	/* r3 is now CPU number  0=master, 1=slave */
	bne	secondary_hold

	b	start_master

	
	/* Exception and interrupt vectors */

	. = _start + 0x200 /* Machine check */
	mtspr	SPRG1, r3
	li r3,0x200
	b general_exception

	. = _start + 0x300 /* DSI */
	mtspr	SPRG1, r3
	li r3,0x300
	b general_exception

	. = _start + 0x380 /* Data segment */
	mtspr	SPRG1, r3
	li r3,0x380
	b general_exception

	. = _start + 0x400 /* ISI */
	mtspr	SPRG1, r3
	li r3,0x400
	b general_exception

	. = _start + 0x480 /* Instruction segment */
	mtspr	SPRG1, r3
	li r3,0x480
	b general_exception

	. = _start + 0x500 /* External interrupt */
	mtspr	SPRG1, r3
	li r3,0x500
	b general_exception

	. = _start + 0x600 /* Alignment */
	mtspr	SPRG1, r3
	li r3,0x600
	b general_exception

	. = _start + 0x700 /* Program */
	mtspr	SPRG1, r3
	li r3,0x700
	b general_exception

	. = _start + 0x800 /* Floating point unavailable */
	mtspr	SPRG1, r3
	li r3,0x800
	b general_exception

	. = _start + 0x900 /* Decrementer */
	mtspr	SPRG1, r3
	li r3,0x900
	b general_exception

	. = _start + 0xa00 /* Reserved 1 */
	mtspr	SPRG1, r3
	li r3,0xa00
	b general_exception

	. = _start + 0xb00 /* Reserved 2 */
	mtspr	SPRG1, r3
	li r3,0xb00
	b general_exception

	. = _start + 0xc00 /* System call */
	mtspr	SPRG1, r3
	li r3,0xc00
	b general_exception

	. = _start + 0xd00 /* Trace */
	mtspr	SPRG1, r3
	li r3,0xd00
	b general_exception

	. = _start + 0xf00 /* Performance monitor */
	mtspr	SPRG1, r3
	li r3,0xf00
	b general_exception


general_exception:
	mtspr	SPRG2, r1
	LOAD_REG_IMMEDIATE(r1,__estack_end)
	rldicr  r1,r1,0,59
	addi	r1,r1,-8*18
	std	r0,0*8(r1)
	mfspr	r0, SPRG2
	std	r0,1*8(r1)
	mfspr	r0, SPRG1
	std	r2,2*8(r1)
	std	r0,3*8(r1)
	mflr	r0
	std	r4,4*8(r1)
	std	r5,5*8(r1)
	std	r6,6*8(r1)
	std	r7,7*8(r1)
	std	r8,8*8(r1)
	std	r9,9*8(r1)
	std	r10,10*8(r1)
	std	r11,11*8(r1)
	std	r12,12*8(r1)
	std	r13,13*8(r1)
	std	r0,14*8(r1)
	mfctr	r0
	std	r0,15*8(r1)
	mfxer	r0
	std	r0,16*8(r1)
	mfcr	r0
	std	r0,17*8(r1)
	mr	r4,r1
	stdu	r1,-128(r1)

	LOAD_REG_IMMEDIATE(r12,ps3ExceptionEntry)
	bl	call_c_func

	addi	r1,r1,128
	ld	r2,2*8(r1)
	ld	r3,3*8(r1)
	ld	r4,4*8(r1)
	ld	r5,5*8(r1)
	ld	r6,6*8(r1)
	ld	r7,7*8(r1)
	ld	r8,8*8(r1)
	ld	r9,9*8(r1)
	ld	r10,10*8(r1)
	ld	r11,11*8(r1)
	ld	r12,12*8(r1)
	ld	r13,13*8(r1)
	ld	r0,14*8(r1)
	mtlr	r0
	ld	r0,15*8(r1)
	mtctr	r0
	ld	r0,16*8(r1)
	mtxer	r0
	ld	r0,17*8(r1)
	mtcr	r0
	ld	r0,0*8(r1)
	ld	r1,1*8(r1)
	rfid


relocate_and_start_master:
	/* This is the code which moves us to address 0 if
	   we're started from kboot */
	/* This code needs to be position independent */

	bl	.setmsr

	lis	r3,_start@highest  /* Target address */
	rldicr	r3,r3,32,31
	/* The source address is already in r4 */

	li      r5,copy_to_here@l       /* # bytes of memory to copy */
	li	r6,0x100		/* Start offset, the first 0x100 */
					/* bytes were copied earlier.	 */

	bl	.copy_and_flush		/* copy the first n bytes	 */
					/* this includes the code being	 */
					/* executed here.		 */

	ori	r0,r3,4f@l		/* Jump to the copy of this code */
	mtctr	r0			/* that we just made/relocated	 */
	bctr
4:
	LOAD_REG_IMMEDIATE(r5,_edata)
	sub	r5,r5,r3
	bl	.copy_and_flush		/* copy the rest */

	b  start_master

.setmsr:
	/* Set default MSR */
	LOAD_REG_IMMEDIATE(r3,MSR_SF|MSR_ISF|MSR_ME|MSR_RI|MSR_FP)
	mtmsr	r3
	sync
	isync
	blr


.copy_and_flush:
	addi	r5,r5,-8
	addi	r6,r6,-8
4:	li	r0,16			/* Use the least common		*/
					/* denominator cache line	*/
					/* size.  This results in	*/
					/* extra cache line flushes	*/
					/* but operation is correct.	*/

	mtctr	r0			/* put # words/line in ctr	*/
3:	addi	r6,r6,8			/* copy a cache line		*/
	ldx	r0,r6,r4
	stdx	r0,r6,r3
	bdnz	3b
	dcbst	r6,r3			/* write it to memory		*/
	sync
	icbi	r6,r3			/* flush the icache line	*/
	cmpld	0,r6,r5
	blt	4b
	sync
	addi	r5,r5,8
	addi	r6,r6,8
	blr

	.balign 8
copy_to_here:

start_master:
	bl	.setmsr
	
	/* NOP out the kboot entry vector */
	lis	r3,_start@highest
	rldicr	r3,r3,32,31
	lis	r0,0x60000000@h	/* nop */
	stw	r0,0(r3)
	dcbst	0,r3
	sync
	icbi	0,r3
	isync
		
	/* Clear out the BSS */
	LOAD_REG_IMMEDIATE(r11,_end)
	LOAD_REG_IMMEDIATE(r8,__bss_start)
	sub	r11,r11,r8		/* bss size			*/
	addi	r11,r11,7		/* round up to an even double word */
	rldicl. r11,r11,61,3		/* shift right by 3		*/
	beq	4f
	addi	r8,r8,-8
	li	r0,0
	mtctr	r11			/* zero this many doublewords	*/
3:	stdu	r0,8(r8)
	bdnz	3b
4:	
	sync
	isync
		
	# Set thread id
	LOAD_REG_IMMEDIATE(r3, ps3RootThread)
	mtspr	SPRG3, r3
	
	# Setup stack
	LOAD_REG_IMMEDIATE(r1,__stack_end)

	# Align the stack and create space for the initial activation record 
	rldicr  r1,r1,0,59
	stdu	r1,-128(r1)

	# Zero out LR, to catch bogus returns 
	li	r0,0
	mtlr	r0
	std	r0,0(r1)

	LOAD_REG_IMMEDIATE(r12,__libc_init_array)
	bl	call_c_func

	# Now we can enable the secondary thread
	LOAD_REG_IMMEDIATE(r3, start_slave)
	bl	dispatch_slave
	
	LOAD_REG_IMMEDIATE(r12,main)
	li	r3,0
	li	r4,0
	bl	call_c_func

	LOAD_REG_IMMEDIATE(r12,__libc_fini_array)
	bl	call_c_func
		
1:
	b	1b


start_slave:
	or r2,r2,r2 /* normal prio */

	bl	.setmsr

	# No thread id yet...
	li	r3, 0
	mtspr	SPRG3, r3
	
	# Setup stack
	LOAD_REG_IMMEDIATE(r1,__stack2_end)

	# Align the stack and create space for the initial activation record 
	rldicr  r1,r1,0,59
	stdu	r1,-128(r1)

	# Zero out LR, to catch bogus returns 
	li	r0,0
	mtlr	r0
	std	r0,0(r1)

	LOAD_REG_IMMEDIATE(r12,ps3ThreadSlaveEntry)
	bl	call_c_func

1:
	b	1b


call_c_func:
	ld	r11,0(r12)
	ld	r2,8(r12)
	mtctr	r11
	ld	r11,16(r12)
	bctr
	
	.section ".bss"

	.align 3

__stack:
	.space 65536
__stack_end:

	# This stack is only used for running the scheduler
	# in the secondary thread until we have something proper
	# to run, so keeping it small is fine
__stack2:
	.space 1024
__stack2_end:

	# Stack for the exception handler
__estack:
	.space 8192
__estack_end:

	
	.end

