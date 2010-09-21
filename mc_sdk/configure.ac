#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([ps3sdk], [PS3SDK_VERSION], [])
AC_CONFIG_SRCDIR([src/base/build.mak])
AC_CONFIG_HEADER([config.h])
AC_PS3SDK_VERSION
AC_PS3DEV_PATH

AM_INIT_AUTOMAKE([ps3sdk], [PS3SDK_VERSION])

# Checks for programs.
AC_PS3DEV_TOOLCHAIN
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AS
AC_PROG_RANLIB

# Override the default prefix to point to where the ps3dev tools should be installed.
# TODO: We should see if the user will ever want to override this.
prefix="$ps3dev"

# Create the ps3sdk installation paths.
ps3sdk_topdir="ps3sdk"
ps3sdk="$prefix/$ps3sdk_topdir"
ps3sdk_includedir="$ps3sdk/ppu/include"
ps3sdk_libdir="$ps3sdk/ppu/lib"
PS3SDK="$ps3sdk"
PS3SDK_INCLUDEDIR="$ps3sdk_includedir"
PS3SDK_LIBDIR="$ps3sdk_libdir"
AC_DEFINE_UNQUOTED(PS3SDK_TOPDIR, ["$ps3sdk_topdir"],
          [Toplevel directory where PS3SDK will be installed, relative to $prefix])
AC_SUBST(PS3SDK)
AC_SUBST(PS3SDK_INCLUDEDIR)
AC_SUBST(PS3SDK_LIBDIR)

# CFLAGS and CXXFLAGS used to build ps3sdk libraries.
PS3SDK_CFLAGS="$CFLAGS -G0 -Wall"
PS3SDK_CXXFLAGS="$PS3SDK_CFLAGS -fno-exceptions -fno-rtti"
AC_SUBST(PS3SDK_CFLAGS)
AC_SUBST(PS3SDK_CXXFLAGS)


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/base/Makefile
                 src/av/Makefile
		 src/console/Makefile
                 src/gpu/Makefile
                 src/hypervisor/Makefile
		 src/kernel/Makefile
		 src/network/Makefile
                 src/startup/Makefile
                 src/vuart/Makefile
		 src/newlib_glue/Makefile
		 src/posix/Makefile
		 src/lwip/Makefile
                 src/samples/Makefile])
AC_OUTPUT
