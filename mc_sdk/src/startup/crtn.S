/*
 * PS3 Software Development Kit
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PS3SDK root for details.
 *
 * crtn.S - Tail for .init and .fini sections
 *
 * Copyright (c) 2008 Marcus Comstedt <marcus@mc.pp.se>
 */

	.section ".init"

	.balign	8
	
	addi	r1,r1,112
	ld	r0,16(r1)
	mtlr	r0
	blr
	
	.section ".fini"

	.balign	8
	
	addi	r1,r1,112
	ld	r0,16(r1)
	mtlr	r0
	blr
	
	.end

