SUBDIRS = scripting wxscintilla resources

INCLUDES = $(WX_CXXFLAGS) \
			-I$(top_srcdir)/src/include \
			-I$(top_srcdir)/src/sdk/wxscintilla/include \
			-I$(top_srcdir)/src/include/tinyxml \
			-I$(top_srcdir)/src/include/scripting/include \
 			-I$(top_srcdir)/src/include/scripting/sqplus \
			-I$(top_srcdir)/src/include/mozilla_chardet

lib_LTLIBRARIES = libcodeblocks.la

libcodeblocks_la_LDFLAGS = -version-info @CODEBLOCKS_VERSION_INFO@

libcodeblocks_la_LIBADD =  $(WX_LIBS) \
			scripting/bindings/libsqbindings.la \
			scripting/sqplus/libsqplus.la \
			scripting/sqstdlib/libsqstdlib.la \
			scripting/squirrel/libsquirrel.la \
			../base/tinyxml/libtinyxml.la \
			wxscintilla/libwxscintilla.la

libcodeblocks_la_SOURCES = annoyingdialog.cpp \
						autodetectcompilers.cpp \
						base64.cpp \
						blockallocated.cpp \
						cbauibook.cpp \
						cbexception.cpp \
						cbeditor.cpp \
						cbeditorprintout.cpp \
						cbplugin.cpp \
						cbproject.cpp \
						cbstyledtextctrl.cpp \
						cbthreadpool.cpp \
						cbworkspace.cpp \
						compileoptionsbase.cpp \
						compiler.cpp \
						compilercommandgenerator.cpp \
						compilerfactory.cpp \
						compileroptions.cpp \
						compiletargetbase.cpp \
						configmanager.cpp \
						configmanager-revision.cpp \
						configurationpanel.cpp \
						configuretoolsdlg.cpp \
						confirmreplacedlg.cpp \
						crc32.cpp \
						editarrayfiledlg.cpp \
						editarrayorderdlg.cpp \
						editarraystringdlg.cpp \
						editkeywordsdlg.cpp \
						editorbase.cpp \
						editorcolourset.cpp \
						editorconfigurationdlg.cpp \
						editorlexerloader.cpp \
						editormanager.cpp \
						editor_hooks.cpp \
						editpairdlg.cpp \
						editpathdlg.cpp \
						edittooldlg.cpp \
						encodingdetector.cpp \
						externaldepsdlg.cpp \
						filefilters.cpp \
						filegroupsandmasks.cpp \
						filemanager.cpp \
						finddlg.cpp \
						genericmultilinenotesdlg.cpp \
						globals.cpp \
						importers_globals.cpp \
						incrementalselectlistdlg.cpp \
						infowindow.cpp \
						loggers.cpp \
						logmanager.cpp \
						macrosmanager.cpp \
						managedthread.cpp \
						manager.cpp \
						menuitemsmanager.cpp \
						multiselectdlg.cpp \
						newfromtemplatedlg.cpp \
						personalitymanager.cpp \
						pipedprocess.cpp \
						pluginmanager.cpp \
						pluginsconfigurationdlg.cpp \
						printing_types.cpp \
						projectbuildtarget.cpp \
						projectdepsdlg.cpp \
						projectfile.cpp \
						projectfileoptionsdlg.cpp \
						projectlayoutloader.cpp \
						projectloader.cpp \
						projectloader_hooks.cpp \
						projectmanager.cpp \
						projectoptionsdlg.cpp \
						projectsfilemasksdlg.cpp \
						projecttemplateloader.cpp \
						replacedlg.cpp \
						scriptingmanager.cpp \
						scriptsecuritywarningdlg.cpp \
						scrollingdialog.cpp \
						sdk_events.cpp \
						searchresultslog.cpp \
						selecttargetdlg.cpp \
						templatemanager.cpp \
						toolsmanager.cpp \
						uservarmanager.cpp \
						virtualbuildtargetsdlg.cpp \
						workspaceloader.cpp \
						xtra_res.cpp  \
						nullptr.cpp \
						mozilla_chardet/src/CharDistribution.cpp \
						mozilla_chardet/src/JpCntx.cpp \
						mozilla_chardet/src/LangBulgarianModel.cpp \
						mozilla_chardet/src/LangCyrillicModel.cpp \
						mozilla_chardet/src/LangGreekModel.cpp \
						mozilla_chardet/src/LangHebrewModel.cpp \
						mozilla_chardet/src/LangHungarianModel.cpp \
						mozilla_chardet/src/LangThaiModel.cpp \
						mozilla_chardet/src/nsBig5Prober.cpp \
						mozilla_chardet/src/nsCharSetProber.cpp \
						mozilla_chardet/src/nsEscCharsetProber.cpp \
						mozilla_chardet/src/nsEscSM.cpp \
						mozilla_chardet/src/nsEUCJPProber.cpp \
						mozilla_chardet/src/nsEUCKRProber.cpp \
						mozilla_chardet/src/nsEUCTWProber.cpp \
						mozilla_chardet/src/nsGB2312Prober.cpp \
						mozilla_chardet/src/nsHebrewProber.cpp \
						mozilla_chardet/src/nsLatin1Prober.cpp \
						mozilla_chardet/src/nsMBCSGroupProber.cpp \
						mozilla_chardet/src/nsMBCSSM.cpp \
						mozilla_chardet/src/nsSBCharSetProber.cpp \
						mozilla_chardet/src/nsSBCSGroupProber.cpp \
						mozilla_chardet/src/nsSJISProber.cpp \
						mozilla_chardet/src/nsUniversalDetector.cpp \
						mozilla_chardet/src/nsUTF8Prober.cpp

EXTRA_DIST =  	mozilla_chardet/src/Big5Freq.tab \
		mozilla_chardet/src/EUCKRFreq.tab \
		mozilla_chardet/src/EUCTWFreq.tab \
		mozilla_chardet/src/GB2312Freq.tab \
		mozilla_chardet/src/JISFreq.tab
