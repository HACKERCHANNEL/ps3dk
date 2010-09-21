libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
CCAS = $(CC) -mregnames
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/hypervisor -I$(top_srcdir)/src/startup

lib_LIBRARIES = libps3kernel.a

noinst_HEADERS = threadInternal.h

libps3kernel_a_SOURCES = realmode.c mmu.c mmuonoff.S mutex.c rootthread.c \
	tb.c scheduler.c sleep.c sleep_util.c ctxswitch.S threadalloc.c \
	bootdata.c exception.c debug.c

libps3kernelincludedir = @PS3SDK_INCLUDEDIR@
libps3kernelinclude_HEADERS = ps3mmu.h ps3thread.h ps3tb.h ps3bootdata.h \
	ps3realmode.h ps3lpar.h ps3debug.h
