include ../../Make.vars

CONFIGURE_DEPENCIES = $(srcdir)/Makefile.am

LOBAFX_LIB_VERSION = 1:0:0

EXTRA_DIST =                                    \
    meta.json

pkglib_LTLIBRARIES =                            \
    lobafx.la

lobafx_la_CPPFLAGS =                            \
    $(AM_CPPFLAGS)                              \
    $(TBB_CPPFLAGS)                             \
    -I$(top_srcdir)/src/coreapps

lobafx_la_SOURCES =                             \
    main.cc                                     \
    nox_nal.hh                                  \
    nox_nal.cc                                  \
    nal.hh                                      \
    core.cc                                     \
    core.hh                                     \
    app.hh                                      \
    myapps.cc                                   \
    myapps.hh                                   \
    lang.hh                                     \
    lang.cc                                     \
    candidate.hh                                \
    lib.hh                                      \
    lib.cc                                      \
    nox_impl.hh                                 \
    nox_impl.cc

lobafx_la_LDFLAGS =                             \
    $(AM_LDFLAGS) -module                       \
    -version-info $(LOBAFX_LIB_VERSION)

lobafx_la_LIBADD =                              \
    $(TBB_LDFLAGS)                              \
    $(TBB_LIBS)

NOX_RUNTIMEFILES = meta.json

all-local: nox-all-local
clean-local: nox-clean-local 
install-exec-hook: nox-install-local
