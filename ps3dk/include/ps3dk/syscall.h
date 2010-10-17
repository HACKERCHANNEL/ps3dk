/*
	libps3dk - An open-source PS3 SDK
	Syscall list and invocation

Copyright (C) 2010              Alex Marshall "trap15" <trap15@raidenii.net>

# This code is licensed to you under the terms of the MIT license;
# see file LICENSE or http://www.opensource.org/licenses/mit-license.php
*/

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#define SYSCALL_GPIO_SET						630
#define SYSCALL_GPIO_GET						631

#define SYSCALL_IN_0(ins)
#define SYSCALL_IN_1(ins)						\
	__p1 = ins[0];
#define SYSCALL_IN_2(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];
#define SYSCALL_IN_3(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];							\
	__p3 = ins[2];
#define SYSCALL_IN_4(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];							\
	__p3 = ins[2];							\
	__p4 = ins[3];
#define SYSCALL_IN_5(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];							\
	__p3 = ins[2];							\
	__p4 = ins[3];							\
	__p5 = ins[4];
#define SYSCALL_IN_6(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];							\
	__p3 = ins[2];							\
	__p4 = ins[3];							\
	__p5 = ins[4];							\
	__p6 = ins[5];
#define SYSCALL_IN_7(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];							\
	__p3 = ins[2];							\
	__p4 = ins[3];							\
	__p5 = ins[4];							\
	__p6 = ins[5];							\
	__p7 = ins[6];
#define SYSCALL_IN_8(ins)						\
	__p1 = ins[0];							\
	__p2 = ins[1];							\
	__p3 = ins[2];							\
	__p4 = ins[3];							\
	__p5 = ins[4];							\
	__p6 = ins[5];							\
	__p7 = ins[6];							\
	__p8 = ins[7];

#define SYSCALL_OUT_0(outs)
#define SYSCALL_OUT_1(outs)						\
	outs[0] = __p1;
#define SYSCALL_OUT_2(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;
#define SYSCALL_OUT_3(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;							\
	outs[2] = __p3;
#define SYSCALL_OUT_4(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;							\
	outs[2] = __p3;							\
	outs[3] = __p4;
#define SYSCALL_OUT_5(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;							\
	outs[2] = __p3;							\
	outs[3] = __p4;							\
	outs[4] = __p5;
#define SYSCALL_OUT_6(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;							\
	outs[2] = __p3;							\
	outs[3] = __p4;							\
	outs[4] = __p5;							\
	outs[5] = __p6;
#define SYSCALL_OUT_7(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;							\
	outs[2] = __p3;							\
	outs[3] = __p4;							\
	outs[4] = __p5;							\
	outs[5] = __p6;							\
	outs[6] = __p7;
#define SYSCALL_OUT_8(outs)						\
	outs[0] = __p1;							\
	outs[1] = __p2;							\
	outs[2] = __p3;							\
	outs[3] = __p4;							\
	outs[4] = __p5;							\
	outs[5] = __p6;							\
	outs[6] = __p7;							\
	outs[7] = __p8;

#define DO_SYSCALL(num, i, ins, o, outs) do {				\
register unsigned long int __p1 __asm__ ("3");				\
register unsigned long int __p2 __asm__ ("4");				\
register unsigned long int __p3 __asm__ ("5");				\
register unsigned long int __p4 __asm__ ("6");				\
register unsigned long int __p5 __asm__ ("7");				\
register unsigned long int __p6 __asm__ ("8");				\
register unsigned long int __p7 __asm__ ("9");				\
register unsigned long int __p8 __asm__ ("10");				\
register unsigned long int n  __asm__ ("11") = num;			\
	SYSCALL_IN_##i((ins))						\
__asm__ volatile ("sc"							\
		: "=r"(__p1), "=r"(__p2), "=r"(__p3), "=r"(__p4),	\
		  "=r"(__p5), "=r"(__p6), "=r"(__p7), "=r"(__p8), "=r"(n) \
		: "r" (__p1), "r" (__p2), "r" (__p3), "r" (__p4),	\
		  "r" (__p5), "r" (__p6), "r" (__p7), "r" (__p8), "r" (n) \
		: "0", "12", "lr", "ctr", "xer", "cr0", "cr1", "cr5",	\
		  "cr6", "cr7", "memory");				\
	SYSCALL_OUT_##o((outs))						\
} while(0)

#endif /* _SYSCALL_H_ */

