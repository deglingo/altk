#

AM_CFLAGS = -I$(top_srcdir) @CLOG_CFLAGS@ @LOS_CFLAGS@ @AL5_CFLAGS@

AM_LDFLAGS = @CLOG_LIBS@ @LOS_LIBS@ @AL5_LIBS@ $(top_builddir)/altk/libaltk.la

noinst_PROGRAMS = \
	ex-label

ex_label_CPPFLAGS = -DCL_LOG_DOMAIN=\"ex-label\"

# [fixme] find a way to not repeat this for every prog
EXTRA_ex_label_DEPENDENCIES = $(top_builddir)/altk/libaltk.la
