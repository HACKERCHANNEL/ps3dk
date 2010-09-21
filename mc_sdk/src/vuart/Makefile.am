libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/kernel \
	-I$(top_srcdir)/src/hypervisor

lib_LIBRARIES = libps3vuart.a

libps3vuart_a_SOURCES = read.c write.c

libps3vuartincludedir = @PS3SDK_INCLUDEDIR@
libps3vuartinclude_HEADERS = ps3vuart.h
