#

AM_CFLAGS = -I$(top_srcdir) -I$(top_builddir) @CLOG_CFLAGS@ @LOS_CFLAGS@ @AL5_CFLAGS@

AM_LDFLAGS = @CLOG_LIBS@ @LOS_LIBS@ @AL5_LIBS@ $(top_builddir)/altk/libaltk.la

noinst_PROGRAMS = \
	ex-label \
	ex-button \
	ex-box \
	ex-table \
	ex-builder

ex_label_CPPFLAGS = -DCL_LOG_DOMAIN=\"ex-label\"
ex_button_CPPFLAGS = -DCL_LOG_DOMAIN=\"ex-button\"
ex_box_CPPFLAGS = -DCL_LOG_DOMAIN=\"ex-box\"
ex_table_CPPFLAGS = -DCL_LOG_DOMAIN=\"ex-table\"
ex_builder_CPPFLAGS = -DCL_LOG_DOMAIN=\"ex-builder\"

# [fixme] find a way to not repeat this for every prog
EXTRA_ex_label_DEPENDENCIES = $(top_builddir)/altk/libaltk.la
EXTRA_ex_button_DEPENDENCIES = $(top_builddir)/altk/libaltk.la
EXTRA_ex_box_DEPENDENCIES = $(top_builddir)/altk/libaltk.la
EXTRA_ex_table_DEPENDENCIES = $(top_builddir)/altk/libaltk.la
EXTRA_ex_builder_DEPENDENCIES = $(top_builddir)/altk/libaltk.la

ex_builder_SOURCES = \
	ex-builder.c \
	ex-builder-dialog.c

BUILT_SOURCES = \
	ex-builder-dialog.h ex-builder-dialog.c

ex-builder-dialog.h ex-builder-dialog.c \
: ex-builder-dialog.stamp-h

ex-builder-dialog.stamp-h: ex-builder-dialog.xml
	$(top_builddir)/tools/altk-xml2c -o ex-builder-dialog -n EX_BUILDER_DIALOG $<
	echo >$@
