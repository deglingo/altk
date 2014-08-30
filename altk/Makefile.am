#

lib_LTLIBRARIES = libaltk.la

include_HEADERS = altk.h

includealtk_HEADERS = \
	altkbase.h \
	altkregion.h \
	altkevent.h \
	altkmain.h \
	altkdrawable.h altkdrawable-def.h \
	altkdisplay.h altkdisplay-def.h \
	altkstyle.h altkstyle-def.h \
	altkwidget.h altkwidget-def.h \
	altklabel.h altklabel-def.h \
	altkcontainer.h altkcontainer-def.h \
	altkbin.h altkbin-def.h \
	altkdialog.h altkdialog-def.h

libaltk_la_CFLAGS = -DCL_LOG_DOMAIN=\"altk\" -I$(top_srcdir) -I$(top_builddir) @CLOG_CFLAGS@ @LOS_CFLAGS@ @GLIB_CFLAGS@

libaltk_la_SOURCES = \
	altkbase.c \
	altkregion.c \
	altkevent.c \
	altkmain.c \
	altkdrawable.c \
	altkdisplay.c \
	altkstyle.c \
	altkwidget.c \
	altklabel.c \
	altkcontainer.c \
	altkbin.c \
	altkdialog.c

BUILT_SOURCES = \
	altkdrawable-def.h altkdrawable.inl \
	altkdisplay-def.h altkdisplay.inl \
	altkstyle-def.h altkstyle.inl \
	altkwidget-def.h altkwidget.inl \
	altklabel-def.h altklabel.inl \
	altkcontainer-def.h altkcontainer.inl \
	altkbin-def.h altkbin.inl \
	altkdialog-def.h altkdialog.inl

altkdrawable-def.h altkdrawable.inl \
altkdisplay-def.h altkdisplay.inl \
altkstyle-def.h altkstyle.inl \
altkwidget-def.h altkwidget.inl \
altklabel-def.h altklabel.inl \
altkcontainer-def.h altkcontainer.inl \
altkbin-def.h altkbin.inl \
altkdialog-def.h altkdialog.inl \
: altktypes.stamp-h

altktypes.stamp-h: altktypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
