SUBDIRS = resources

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = libValgrind.la

libValgrind_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libValgrind_la_LIBADD = $(PLUGIN_WX_LIBS) ../../../sdk/libcodeblocks.la

libValgrind_la_SOURCES = Valgrind.cpp \
            ValgrindListLog.cpp

noinst_HEADERS = Valgrind.h \
            ValgrindListLog.h

EXTRA_DIST = $(srcdir)/*.cbp
