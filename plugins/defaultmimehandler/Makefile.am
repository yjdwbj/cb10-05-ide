SUBDIRS = resources

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = libdefaultmimehandler.la

libdefaultmimehandler_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libdefaultmimehandler_la_LIBADD = $(PLUGIN_WX_LIBS) ../../sdk/libcodeblocks.la

libdefaultmimehandler_la_SOURCES = defaultmimehandler.cpp \
			editmimetypesdlg.cpp \
			EmbeddedHtmlPanel.cpp

noinst_HEADERS = defaultmimehandler.h \
			editmimetypesdlg.h \
			mimetypesarray.h \
			EmbeddedHtmlPanel.h
