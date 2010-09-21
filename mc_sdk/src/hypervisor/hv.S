/*
 * arch/ppc64/kernel/lvn_hv.S
 *
 * Hypervisor interface for BPA.
 *
 * Author: Frank Rowand frowand@mvista.com or source@mvista.com
 *
 * 2003, 2004 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#define XGLUE(a,b) a##b
#define GLUE(a,b) XGLUE(a,b)

#define LV1CALL_OPCODE	0x44000022

#define lv1call	.long LV1CALL_OPCODE; extsw r3, r3

#define _GLOBAL(name) \
	.section ".text"; \
	.align 2 ; \
	.globl name; \
	.globl GLUE(.,name); \
	.section ".opd","aw"; \
name: \
	.quad GLUE(.,name); \
	.quad .TOC.@tocbase; \
	.quad 0; \
	.previous; \
	.type GLUE(.,name),@function; \
GLUE(.,name):


#define LV1_N_IN_0_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr


#define LV1_0_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu    r3, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_0_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r3, -8(r1);			\
	stdu	r4, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_0_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r3, -8(r1);			\
	std	r4, -16(r1);			\
	stdu	r5, -24(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 24;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_0_IN_7_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r3, -8(r1);			\
	std	r4, -16(r1);			\
	std	r5, -24(r1);			\
	std	r6, -32(r1);			\
	std	r7, -40(r1);			\
	std	r8, -48(r1);			\
	stdu	r9, -56(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 56;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
	ld	r11, -40(r1);			\
	std	r8, 0(r11);			\
	ld	r11, -48(r1);			\
	std	r9, 0(r11);			\
	ld	r11, -56(r1);			\
	std	r10, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu    r4, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r4, -8(r1);			\
	stdu	r5, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r4, -8(r1);			\
	std	r5, -16(r1);			\
	stdu	r6, -24(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 24;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_4_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r4, -8(r1);			\
	std	r5, -16(r1);			\
	std	r6, -24(r1);			\
	stdu	r7, -32(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 32;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_5_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r4, -8(r1);			\
	std	r5, -16(r1);			\
	std	r6, -24(r1);			\
	std	r7, -32(r1);			\
	stdu	r8, -40(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 40;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
	ld	r11, -40(r1);			\
	std	r8, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_6_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r4, -8(r1);			\
	std	r5, -16(r1);			\
	std	r6, -24(r1);			\
	std	r7, -32(r1);			\
	std	r8, -40(r1);			\
	stdu	r9, -48(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 48;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
	ld	r11, -40(r1);			\
	std	r8, 0(r11);			\
	ld	r11, -48(r1);			\
	std	r9, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_1_IN_7_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r4, -8(r1);			\
	std	r5, -16(r1);			\
	std	r6, -24(r1);			\
	std	r7, -32(r1);			\
	std	r8, -40(r1);			\
	std	r9, -48(r1);			\
	stdu	r10, -56(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 56;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
	ld	r11, -40(r1);			\
	std	r8, 0(r11);			\
	ld	r11, -48(r1);			\
	std	r9, 0(r11);			\
	ld	r11, -56(r1);			\
	std	r10, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_2_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu	r5, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_2_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r5, -8(r1);			\
	stdu	r6, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_2_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r5, -8(r1);			\
	std	r6, -16(r1);			\
	stdu	r7, -24(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 24;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_2_IN_4_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r5, -8(r1);			\
	std	r6, -16(r1);			\
	std	r7, -24(r1);			\
	stdu	r8, -32(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 32;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_2_IN_5_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r5, -8(r1);			\
	std	r6, -16(r1);			\
	std	r7, -24(r1);			\
	std	r8, -32(r1);			\
	stdu	r9, -40(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 40;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
	ld	r11, -32(r1);			\
	std	r7, 0(r11);			\
	ld	r11, -40(r1);			\
	std	r8, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_3_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu	r6, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_3_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r6, -8(r1);			\
	stdu	r7, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_3_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r6, -8(r1);			\
	std	r7, -16(r1);			\
	stdu	r8, -24(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 24;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_4_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu    r7, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_4_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r7, -8(r1);			\
	stdu	r8, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_4_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r7, -8(r1);			\
	std	r8, -16(r1);			\
	stdu	r9, -24(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 24;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_5_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu    r8, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_5_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r8, -8(r1);			\
	stdu	r9, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_5_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r8, -8(r1);			\
	std	r9, -16(r1);			\
	stdu	r10, -24(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 24;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, -24(r1);			\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_6_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu    r9, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_6_IN_2_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r9, -8(r1);			\
	stdu    r10, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_6_IN_3_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std     r9, -8(r1);			\
	stdu    r10, -16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 16;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
	ld	r11, -16(r1);			\
	std	r5, 0(r11);			\
	ld	r11, 48+8*8(r1);		\
	std	r6, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_7_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	stdu    r10, -8(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	addi	r1, r1, 8;			\
	ld	r11, -8(r1);			\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_7_IN_6_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	std	r10, 48+8*7(r1);		\
						\
	li	r11, API_NUMBER;		\
	lv1call;				\
						\
	ld	r11, 48+8*7(r1);		\
	std	r4, 0(r11);			\
	ld	r11, 48+8*8(r1);		\
	std	r5, 0(r11);			\
	ld	r11, 48+8*9(r1);		\
	std	r6, 0(r11);			\
	ld	r11, 48+8*10(r1);		\
	std	r7, 0(r11);			\
	ld	r11, 48+8*11(r1);		\
	std	r8, 0(r11);			\
	ld	r11, 48+8*12(r1);		\
	std	r9, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr

#define LV1_8_IN_1_OUT(API_NAME, API_NUMBER)	\
_GLOBAL(API_NAME)				\
						\
	mflr	r0;				\
	std	r0, 16(r1);			\
						\
	li      r11, API_NUMBER;		\
	lv1call;				\
						\
	ld	r11, 48+8*8(r1);		\
	std	r4, 0(r11);			\
						\
	ld	r0, 16(r1);			\
	mtlr	r0;				\
	blr


	.text

#define LV1_0_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_1_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_2_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_3_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_4_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_5_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_6_IN_0_OUT LV1_N_IN_0_OUT
#define LV1_7_IN_0_OUT LV1_N_IN_0_OUT

#define CHECK_SELECTED(NAME) F_##NAME
	
#define LV1_CALL(NAME, IN, OUT, NUM) \
	.ifc F_##NAME,1;				\
	LV1_##IN##_IN_##OUT##_OUT(lv1_##NAME, NUM);	\
	.endif

#include "lv1calltab.h"

#undef LV1_CALL
