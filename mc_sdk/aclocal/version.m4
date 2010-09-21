dnl
dnl AC_PS3SDK_VERSION()
dnl
dnl Determine the ps3sdk package version.

AC_DEFUN([AC_PS3SDK_VERSION],
[
  AC_BEFORE([$0], [AM_INIT_AUTOMAKE])

  AC_MSG_CHECKING([for ps3sdk version])
  AS_IF([test -r "${srcdir}/aclocal/version.m4"],
        [],
        [AC_MSG_ERROR([Unable to find aclocal/version.m4])])
  AS_IF([test -r "${srcdir}/VERSION"],
        [],
        [AC_MSG_ERROR([Unable to find VERSION])])
  ps3sdk_version=`cat "${srcdir}/VERSION"`
  AC_MSG_RESULT($ps3sdk_version)
])
