libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/vuart

lib_LIBRARIES = libps3av.a

noinst_HEADERS = avInternal.h

libps3av_a_SOURCES = init.c avb.c avInternal.c

libps3avincludedir = @PS3SDK_INCLUDEDIR@
libps3avinclude_HEADERS = ps3av.h
