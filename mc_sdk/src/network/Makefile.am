libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/kernel \
	-I$(top_srcdir)/src/hypervisor

lib_LIBRARIES = libps3network.a

noinst_HEADERS = ethInternal.h

libps3network_a_SOURCES = eth.c

libps3networkincludedir = @PS3SDK_INCLUDEDIR@
libps3networkinclude_HEADERS = ps3network.h
