libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/kernel -I$(top_srcdir)/src/hypervisor

lib_LIBRARIES = libps3gpu.a

libps3gpu_a_SOURCES = init.c

libps3gpuincludedir = @PS3SDK_INCLUDEDIR@
libps3gpuinclude_HEADERS = ps3gpu.h
