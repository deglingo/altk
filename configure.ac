AC_INIT([altk], [0.0.0])
AM_INIT_AUTOMAKE

AC_SUBST([pkgconfigdir], ['${libdir}/pkgconfig'])
AC_SUBST([includealtkdir], ['${includedir}/altk'])

: ${CFLAGS:=-g -Wall}

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT

PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([LOS], [los])
AC_PATH_PROG([LOS_GENTYPES], [los-gentypes])
test x"$LOS_GENTYPES" != x || AC_ERROR([los-gentypes not found])
PKG_CHECK_MODULES([AL5], [allegro-debug-5 allegro_main-debug-5])

AC_CONFIG_FILES([ \
  Makefile \
  altk/Makefile \
])
AC_CONFIG_FILES([altk.pc])
AC_OUTPUT
