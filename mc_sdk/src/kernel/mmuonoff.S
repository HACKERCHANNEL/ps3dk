/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * mmuonoff.S - Enable/disable MMU
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

#include <spr.h>

	.global ps3MmuOff, ps3MmuOn

	.text

.__mmu_off:
        mfmsr   r3
        andi.   r0,r3,MSR_IR|MSR_DR
        beqlr
        mflr    r4
        andc    r3,r3,r0
        mtspr   SRR0,r4
        mtspr   SRR1,r3
        sync
        rfid
        b       .       /* prevent speculative execution */


.__mmu_on:
	mfmsr	r0
	ori	r0, r0, MSR_IR | MSR_DR
        mtspr   SRR1,r0
        mflr    r0
        mtspr   SRR0,r0
        sync
        rfid
        b       .       /* prevent speculative execution */


        .section ".opd","aw"

ps3MmuOff:
        .llong .__mmu_off
        .llong .TOC.@tocbase
        .llong 0

ps3MmuOn:
        .llong .__mmu_on
        .llong .TOC.@tocbase
        .llong 0

	.end

