#

lib_LTLIBRARIES = libaltk.la

include_HEADERS = altk.h

includealtk_HEADERS = \
	altkbase.h \
	altkregion.h \
	altkevent.h \
	altkmain.h \
	altkfont.h altkfont-def.h \
	altkgc.h altkgc-def.h \
	altkdrawable.h altkdrawable-def.h \
	altkwindow.h altkwindow-def.h \
	altkwm.h \
	altkbitmap.h altkbitmap-def.h \
	altkdisplay.h altkdisplay-def.h \
	altkstyle.h altkstyle-def.h \
	altkstylecontext.h altkstylecontext-def.h \
	altkbuilder.h altkbuilder-def.h \
	altkwidget.h altkwidget-def.h \
	altklabel.h altklabel-def.h \
	altkcontainer.h altkcontainer-def.h \
	altkbin.h altkbin-def.h \
	altkbox.h altkbox-def.h \
	altkbutton.h altkbutton-def.h \
	altkdialog.h altkdialog-def.h

libaltk_la_CFLAGS = -DCL_LOG_DOMAIN=\"altk\" -I$(top_srcdir) -I$(top_builddir) @CLOG_CFLAGS@ @LOS_CFLAGS@ @GLIB_CFLAGS@

libaltk_la_SOURCES = \
	altkbase.c \
	altkregion.c \
	altkevent.c \
	altkmain.c \
	altkfont.c \
	altkgc.c \
	altkdrawable.c \
	altkwindow.c \
	altkwm.c \
	altkbitmap.c \
	altkdisplay.c \
	altkstyle.c \
	altkstylecontext.c \
	altkbuilder.c \
	altkwidget.c \
	altklabel.c \
	altkcontainer.c \
	altkbin.c \
	altkbox.c \
	altktable.c \
	altkbutton.c \
	altkdialog.c

BUILT_SOURCES = \
	altkfont-def.h altkfont.inl \
	altkgc-def.h altkgc.inl \
	altkdrawable-def.h altkdrawable.inl \
	altkwindow-def.h altkwindow.inl \
	altkbitmap-def.h altkbitmap.inl \
	altkdisplay-def.h altkdisplay.inl \
	altkstyle-def.h altkstyle.inl \
	altkstylecontext-def.h altkstylecontext.inl \
	altkbuilder-def.h altkbuilder.inl \
	altkwidget-def.h altkwidget.inl \
	altklabel-def.h altklabel.inl \
	altkcontainer-def.h altkcontainer.inl \
	altkbin-def.h altkbin.inl \
	altkbox-def.h altkbox.inl \
	altktable-def.h altktable.inl \
	altkbutton-def.h altkbutton.inl \
	altkdialog-def.h altkdialog.inl

altkfont-def.h altkfont.inl \
altkgc-def.h altkgc.inl \
altkdrawable-def.h altkdrawable.inl \
altkwindow-def.h altkwindow.inl \
altkbitmap-def.h altkbitmap.inl \
altkdisplay-def.h altkdisplay.inl \
altkstyle-def.h altkstyle.inl \
altkstylecontext-def.h altkstylecontext.inl \
altkbuilder-def.h altkbuilder.inl \
altkwidget-def.h altkwidget.inl \
altklabel-def.h altklabel.inl \
altkcontainer-def.h altkcontainer.inl \
altkbin-def.h altkbin.inl \
altkbox-def.h altkbox.inl \
altktable-def.h altktable.inl \
altkbutton-def.h altkbutton.inl \
altkdialog-def.h altkdialog.inl \
: altktypes.stamp-h

altktypes.stamp-h: altktypes.in
	cpp $< | $(LOS_GENTYPES)
	echo >$@
