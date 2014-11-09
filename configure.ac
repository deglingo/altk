AC_INIT([altk], [0.0.0])
AM_INIT_AUTOMAKE

AC_SUBST([pkgconfigdir], ['${libdir}/pkgconfig'])
AC_SUBST([includealtkdir], ['${includedir}/altk'])

: ${CFLAGS:=-g -Wall}

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT

PKG_CHECK_MODULES([CLOG], [clog])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([LOS], [los])
AC_PATH_PROG([LOS_GENTYPES], [los-gentypes])
test x"$LOS_GENTYPES" != x || AC_ERROR([los-gentypes not found])
PKG_CHECK_MODULES([AL5], [allegro-debug-5 allegro_main-debug-5 allegro_primitives-debug-5 allegro_image-debug-5 allegro_font-debug-5])

AC_CONFIG_FILES([ \
  Makefile \
  tools/Makefile \
  altk/Makefile \
  examples/Makefile \
])
AC_CONFIG_FILES([altk.pc])
AC_CONFIG_FILES([examples/memcheck], [chmod +x examples/memcheck])
AC_OUTPUT
