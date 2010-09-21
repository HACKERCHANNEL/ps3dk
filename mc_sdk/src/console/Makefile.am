libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/gpu \
		-I$(top_srcdir)/src/kernel

lib_LIBRARIES = libps3console.a

libps3console_a_SOURCES = console.c font.c

libps3consoleincludedir = @PS3SDK_INCLUDEDIR@
libps3consoleinclude_HEADERS = ps3console.h
