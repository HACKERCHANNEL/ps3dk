dnl
dnl AC_PS3DEV_PATH()
dnl
dnl Check for a valid ps3dev installation.

AC_DEFUN([AC_PS3DEV_PATH],
[
  AC_ARG_WITH(ps3dev,
              [  --with-ps3dev=DIR       Path where the ps3dev toolchain is inst
alled (default is $PS3DEV)],
              ps3dev="$withval", ps3dev="$PS3DEV")

  AC_MSG_CHECKING(for ps3dev)
  if test x$ps3dev = x ; then
     # If there's no $PS3DEV environment variable, find out where ppu-gcc lives (it should be on the $PATH).
     ppu_gcc_path=`which ppu-gcc`
     if test x$ppu_gcc_path = x ; then
        AC_MSG_ERROR(can't find the ps3dev toolchain.  Use --with-ps3dev or set 
PS3DEV)
     fi
     # Strip both the /ppu-gcc and /bin and /ppu portions from the path.
     ps3dev=`echo $ppu_gcc_path | sed 's/\/ppu-gcc$//' | sed 's/\/bin$//' | sed 's/\/ppu$//'`
  fi
  AC_MSG_RESULT($ps3dev)

  PS3DEV="$ps3dev"
  AC_SUBST(PS3DEV)

  # Fill out a few common directories for things that need it.
  ps3dev_includedir="$ps3dev/ppu/ppu/include"
  ps3dev_libdir="$ps3dev/ppu/ppu/lib"
  PS3DEV_INCLUDEDIR="$ps3dev_includedir"
  PS3DEV_LIBDIR="$ps3dev_libdir"
  AC_SUBST(PS3DEV_INCLUDEDIR)
  AC_SUBST(PS3DEV_LIBDIR)
])

dnl Check for a tool prefixed with "ppu-".
dnl __PS3DEV_CHECK_PPU_TOOL(VARIABLE, PREFIX, PROG-TO-CHECK-FOR[, VALUE-IF-NOT-FOUND
 [, PATH]])
AC_DEFUN([__PS3DEV_CHECK_PPU_TOOL],
[
  ppu_tool_prefix="ppu-"
  AC_CHECK_PROG($1, ${ppu_tool_prefix}$2, ${ppu_tool_prefix}$2, $3, $4)
])

dnl
dnl AC_PS3DEV_TOOLCHAIN()
dnl
dnl Make sure all of the required ps3dev tools exist.

AC_DEFUN([AC_PS3DEV_TOOLCHAIN],
[
  __PS3DEV_CHECK_PPU_TOOL(PPU_CC, gcc, ppu-gcc)
  __PS3DEV_CHECK_PPU_TOOL(PPU_CXX, g++, ppu-g++)
  __PS3DEV_CHECK_PPU_TOOL(PPU_AS, as, ppu-as)
  __PS3DEV_CHECK_PPU_TOOL(PPU_LD, ld, ppu-ld)
  __PS3DEV_CHECK_PPU_TOOL(PPU_AR, ar, ppu-ar)
  __PS3DEV_CHECK_PPU_TOOL(PPU_NM, nm, ppu-nm)
  __PS3DEV_CHECK_PPU_TOOL(PPU_RANLIB, ranlib, ppu-ranlib)
])
