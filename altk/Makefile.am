#

lib_LTLIBRARIES = libaltk.la

include_HEADERS = altk.h

includealtk_HEADERS = \
	altkbase.h \
	altkmain.h \
	altkwidget.h altkwidget-def.h \
	altkdialog.h

libaltk_la_CFLAGS = -I$(top_srcdir) -I$(top_builddir) @LOS_CFLAGS@ @GLIB_CFLAGS@

libaltk_la_SOURCES = \
	altkbase.c \
	altkmain.c \
	altkwidget.c altkwidget.inl \
	altkdialog.c

BUILT_SOURCES = \
	altkwidget-def.h altkwidget.inl

altkwidget-def.h altkwidget.inl \
: altktypes.stamp-h

altktypes.stamp-h: altktypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
