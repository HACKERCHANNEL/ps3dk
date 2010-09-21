/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * ctxswitch.S - Context switch
 *
 * Copyright (c) 2009 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <spr.h>

#define GPRSAVE(n)   (((n)-14)*8)
#define FPRSAVE(n)   (((n)-14+18)*8)
#define EXTRASAVE(n) (((n)+18+18)*8)
	
	
	.global	ps3ThreadContextSwitch
	.global	ps3ThreadContextInit
	.global ps3ThreadInit
	
	.text

.ps3ThreadContextSwitch:
	/* r3 = new thread
	   r5 = old thread */
	mfspr   r5, SPRG3
	mr.	r3,r3
	
	/* Save old registers */
	mflr	r6
	mfmsr	r7

	std	r14,GPRSAVE(14)(r5)
	std	r15,GPRSAVE(15)(r5)
	std	r16,GPRSAVE(16)(r5)
	std	r17,GPRSAVE(17)(r5)
	std	r18,GPRSAVE(18)(r5)
	std	r19,GPRSAVE(19)(r5)
	std	r20,GPRSAVE(20)(r5)
	std	r21,GPRSAVE(21)(r5)
	std	r22,GPRSAVE(22)(r5)
	std	r23,GPRSAVE(23)(r5)
	std	r24,GPRSAVE(24)(r5)
	std	r25,GPRSAVE(25)(r5)
	std	r26,GPRSAVE(26)(r5)
	std	r27,GPRSAVE(27)(r5)
	std	r28,GPRSAVE(28)(r5)
	std	r29,GPRSAVE(29)(r5)
	std	r30,GPRSAVE(30)(r5)
	std	r31,GPRSAVE(31)(r5)

	stfd	f14,FPRSAVE(14)(r5)
	stfd	f15,FPRSAVE(15)(r5)
	stfd	f16,FPRSAVE(16)(r5)
	stfd	f17,FPRSAVE(17)(r5)
	stfd	f18,FPRSAVE(18)(r5)
	stfd	f19,FPRSAVE(19)(r5)
	stfd	f20,FPRSAVE(20)(r5)
	stfd	f21,FPRSAVE(21)(r5)
	stfd	f22,FPRSAVE(22)(r5)
	stfd	f23,FPRSAVE(23)(r5)
	stfd	f24,FPRSAVE(24)(r5)
	stfd	f25,FPRSAVE(25)(r5)
	stfd	f26,FPRSAVE(26)(r5)
	stfd	f27,FPRSAVE(27)(r5)
	stfd	f28,FPRSAVE(28)(r5)
	stfd	f29,FPRSAVE(29)(r5)
	stfd	f30,FPRSAVE(30)(r5)
	stfd	f31,FPRSAVE(31)(r5)

	std	r1,EXTRASAVE(0)(r5)
	std	r6,EXTRASAVE(1)(r5)
	std	r7,EXTRASAVE(2)(r5)

	bne+	9f

.ps3ThreadContextInit:	
	/* New thread is NULL, wait for a thread to become ready */
	bl	ps3ThreadIdleFunc

9:	
	/* Load new registers */
	ld	r7,EXTRASAVE(2)(r3)
	mtmsr	r7
	isync

	ld	r14,GPRSAVE(14)(r3)
	ld	r15,GPRSAVE(15)(r3)
	ld	r16,GPRSAVE(16)(r3)
	ld	r17,GPRSAVE(17)(r3)
	ld	r18,GPRSAVE(18)(r3)
	ld	r19,GPRSAVE(19)(r3)
	ld	r20,GPRSAVE(20)(r3)
	ld	r21,GPRSAVE(21)(r3)
	ld	r22,GPRSAVE(22)(r3)
	ld	r23,GPRSAVE(23)(r3)
	ld	r24,GPRSAVE(24)(r3)
	ld	r25,GPRSAVE(25)(r3)
	ld	r26,GPRSAVE(26)(r3)
	ld	r27,GPRSAVE(27)(r3)
	ld	r28,GPRSAVE(28)(r3)
	ld	r29,GPRSAVE(29)(r3)
	ld	r30,GPRSAVE(30)(r3)
	ld	r31,GPRSAVE(31)(r3)

	lfd	f14,FPRSAVE(14)(r3)
	lfd	f15,FPRSAVE(15)(r3)
	lfd	f16,FPRSAVE(16)(r3)
	lfd	f17,FPRSAVE(17)(r3)
	lfd	f18,FPRSAVE(18)(r3)
	lfd	f19,FPRSAVE(19)(r3)
	lfd	f20,FPRSAVE(20)(r3)
	lfd	f21,FPRSAVE(21)(r3)
	lfd	f22,FPRSAVE(22)(r3)
	lfd	f23,FPRSAVE(23)(r3)
	lfd	f24,FPRSAVE(24)(r3)
	lfd	f25,FPRSAVE(25)(r3)
	lfd	f26,FPRSAVE(26)(r3)
	lfd	f27,FPRSAVE(27)(r3)
	lfd	f28,FPRSAVE(28)(r3)
	lfd	f29,FPRSAVE(29)(r3)
	lfd	f30,FPRSAVE(30)(r3)
	lfd	f31,FPRSAVE(31)(r3)

	ld	r1,EXTRASAVE(0)(r3)
	ld	r6,EXTRASAVE(1)(r3)

	mtlr	r6

	/* Return to new thread */
	sync
	isync
	mtspr	SPRG3, r3
	stdcx.  r14,0,r3	/* Clear any outstanding reservations */
	blr

.ps3ThreadInit:
	/* r3 = thread
	   r4 = func
	   r5 = arg
	   r6 = stackp */

	std	r4,GPRSAVE(14)(r3)
	std	r5,GPRSAVE(15)(r3)
	std	r6,EXTRASAVE(0)(r3)
	mfmsr	r7
	mflr	r4
	bl	1f
1:	mflr	r5
	addi	r5,r5,2f-1b
	mtlr	r4
	std	r5,EXTRASAVE(1)(r3)
	std	r7,EXTRASAVE(2)(r3)
	blr
2:
	mr	r3,r14
	mr	r4,r15
	li	r14,0
	li	r15,0
	mtlr	r14

	# Align the stack and create space for the initial activation record
	rldicr  r1,r1,0,59
	stdu	r1,-128(r1)

	b	ps3ThreadWrapper
		
		
        .section ".opd","aw"

ps3ThreadContextSwitch:
	.llong .ps3ThreadContextSwitch
	.llong .TOC.@tocbase
	.llong 0

ps3ThreadContextInit:
	.llong .ps3ThreadContextInit
	.llong .TOC.@tocbase
	.llong 0

ps3ThreadInit:
	.llong .ps3ThreadInit
	.llong .TOC.@tocbase
	.llong 0

	.end

