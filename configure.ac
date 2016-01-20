AC_INIT([grep_wizard], [1.0], [williams.mckibbin@gmail.com,jcmjohnmatty@me.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIRS([m4])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AX_WITH_CURSES
if test "x$ax_cv_curses" != xyes || test "x$ax_cv_curses_color" != xyes; then
    AC_MSG_ERROR([requires an X/Open-compatible Curses library with color])
fi
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
 	src/Makefile
])
AC_OUTPUT
