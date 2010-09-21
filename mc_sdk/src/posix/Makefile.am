libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/kernel

lib_LIBRARIES = libps3posix.a

libps3posix_a_SOURCES = sleep.c usleep.c nanosleep.c
