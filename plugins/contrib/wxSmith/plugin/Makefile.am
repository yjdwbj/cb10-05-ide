AM_CPPFLAGS = -DWXMAKINGLIB_PROPGRID

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include \
		-I$(top_srcdir)/src/plugins/contrib/propgrid/include

pluginlib_LTLIBRARIES = libwxsmith.la

pluginlibdir = $(pkglibdir)/plugins

libwxsmith_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libwxsmith_la_LIBADD = $(PLUGIN_WX_LIBS) ../../../../sdk/libcodeblocks.la ../libwxsmithlib.la

libwxsmith_la_SOURCES = wxsmithpluginregistrants.cpp

