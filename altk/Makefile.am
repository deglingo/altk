#

lib_LTLIBRARIES = libaltk.la

include_HEADERS = altk.h

includealtk_HEADERS = \
	altkbase.h \
	altkmain.h \
	altkdisplay.h \
	altkwidget.h altkwidget-def.h \
	altkdialog.h altkdialog-def.h

libaltk_la_CFLAGS = -I$(top_srcdir) -I$(top_builddir) @LOS_CFLAGS@ @GLIB_CFLAGS@

libaltk_la_SOURCES = \
	altkbase.c \
	altkmain.c \
	altkdisplay.c \
	altkwidget.c altkwidget.inl \
	altkdialog.c altkdialog.inl

BUILT_SOURCES = \
	altkwidget-def.h altkwidget.inl \
	altkdialog-def.h altkdialog.inl

altkwidget-def.h altkwidget.inl \
altkdialog-def.h altkdialog.inl \
: altktypes.stamp-h

altktypes.stamp-h: altktypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
