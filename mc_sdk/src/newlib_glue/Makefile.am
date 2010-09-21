libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/kernel \
	-I$(top_srcdir)/src/hypervisor

lib_LIBRARIES = libps3nlglue.a

libps3nlglue_a_SOURCES = _sbrk_r.c _write_r.c _read_r.c _lseek_r.c \
	_fstat_r.c _close_r.c isatty.c _gettimeofday_r.c \
	_getpid_r.c _kill_r.c _exit.c \
	lock.c lock_recursive.c autolock.c


