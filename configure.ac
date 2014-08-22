AC_INIT([altk], [0.0.0])
AM_INIT_AUTOMAKE

AC_SUBST([pkgconfigdir], ['${libdir}/pkgconfig'])

: ${CFLAGS:=-g -Wall}

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT

PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([AL5], [allegro-debug-5 allegro_main-debug-5])

AC_CONFIG_FILES([ \
  Makefile \
])
AC_CONFIG_FILES([altk.pc])
AC_OUTPUT
