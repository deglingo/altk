#

lib_LTLIBRARIES = libaltk.la

include_HEADERS = altk.h

includealtk_HEADERS = \
	altkbase.h \
	altkregion.h \
	altkevent.h \
	altkmain.h \
	altkdisplay.h altkdisplay-def.h \
	altkwidget.h altkwidget-def.h \
	altklabel.h altklabel-def.h \
	altkcontainer.h altkcontainer-def.h \
	altkdialog.h altkdialog-def.h

libaltk_la_CFLAGS = -DCL_LOG_DOMAIN=\"altk\" -I$(top_srcdir) -I$(top_builddir) @CLOG_CFLAGS@ @LOS_CFLAGS@ @GLIB_CFLAGS@

libaltk_la_SOURCES = \
	altkbase.c \
	altkregion.c \
	altkevent.c \
	altkmain.c \
	altkdisplay.c \
	altkwidget.c \
	altklabel.c \
	altkcontainer.c \
	altkdialog.c

BUILT_SOURCES = \
	altkdisplay-def.h altkdisplay.inl \
	altkwidget-def.h altkwidget.inl \
	altklabel-def.h altklabel.inl \
	altkcontainer-def.h altkcontainer.inl \
	altkdialog-def.h altkdialog.inl

altkdisplay-def.h altkdisplay.inl \
altkwidget-def.h altkwidget.inl \
altklabel-def.h altklabel.inl \
altkcontainer-def.h altkcontainer.inl \
altkdialog-def.h altkdialog.inl \
: altktypes.stamp-h

altktypes.stamp-h: altktypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
