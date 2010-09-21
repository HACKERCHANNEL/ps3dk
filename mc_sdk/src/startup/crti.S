/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * crti.S - Head for .init and .fini sections
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

	.global _init, _fini
	
	.section ".init"

	.balign	8
	
._init:
	mflr	r0
	std	r0,16(r1)
	stdu	r1,-112(r1)
	nop

	.section ".fini"
	
	.balign	8

._fini:
	mflr	r0
	std	r0,16(r1)
	stdu	r1,-112(r1)
	nop
	
	.section ".opd","aw"

_init:
	.quad	._init
	.quad	.TOC.@tocbase
	.quad	0

_fini:
	.quad	._fini
	.quad	.TOC.@tocbase
	.quad	0


	.end

