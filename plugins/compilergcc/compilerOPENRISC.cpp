/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 7920 $
 * $Id: compilerOPENRISC.cpp 7920 2012-04-07 09:15:28Z tpetrov $
 * $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/compilergcc/compilerOPENRISC.cpp $
 */

#include <sdk.h>
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
    #include <wx/utils.h> // wxGetOSDirectory, wxGetEnv
#endif
#include <wx/filefn.h> // wxFileExists
#include <wx/fileconf.h> // wxFileConfig
#include "compilerOPENRISC.h"

CompilerOPENRISC::CompilerOPENRISC()
    : Compiler(_("MC Compiler"),_T("mc"),_T("lcy"))
{
    Reset();
}

CompilerOPENRISC::~CompilerOPENRISC()
{
    //dtor
}

Compiler * CompilerOPENRISC::CreateCopy()
{
    Compiler* c = new CompilerOPENRISC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}



void CompilerOPENRISC::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = _T("mc-uclinux-gcc.exe");
        m_Programs.CPP = _T("mc-uclinux-gcc.exe");
        m_Programs.LD = _T("mc-uclinux-ld.exe");

        m_Programs.DBG = _T("mc-elf-gdb.exe");
        m_Programs.DBGconfig = wxEmptyString;
        m_Programs.LIB = _T("mc-uclinux-ar.exe");
        m_Programs.WINDRES = _T("");
        m_Programs.OBJDUMP =_T("mc-uclinux-objdump.exe");
        m_Programs.MAKE = _T("make.exe");
        m_Programs.SIM = _T("mc-uclinux-sim.exe");
        m_Programs.JTAG = _T("jp2.exe");
        m_Programs.FWDownload = _T("mc-download.exe");
        m_Programs.OBJCOPY = _T("mc-uclinux-objcopy.exe");
    }
    else
    {
        m_Programs.C = _T("mc-uclinux-gcc");
        m_Programs.CPP = _T("mc-uclinux-g++");
        m_Programs.LD = _T("mc-uclinux-g++");
        m_Programs.DBG = _T("mc-uclinux-gdb");
        m_Programs.DBGconfig = wxEmptyString;
        m_Programs.LIB = _T("mc-uclinux-ar");
        m_Programs.WINDRES = _T("");
        m_Programs.MAKE = _T("make");
    }
    m_Switches.includeDirs = _T("-I");
    m_Switches.libDirs = _T("-L");
    m_Switches.linkLibs = _T("-l");
    m_Switches.defines = _T("-D");
    m_Switches.genericSwitch = _T("-");
    m_Switches.objectExtension = _T("o");
    m_Switches.needDependencies = true;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = false;
    m_Switches.logging = CompilerSwitches::defaultLogging;
    m_Switches.libPrefix = _T("lib");
    m_Switches.libExtension = _T("a");
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = false;

    // Summary of GCC options: http://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html

    m_Options.ClearOptions();
    m_Options.AddOption(_("Optimize fully (for size)"),
                _T("-Os"),
                _("Optimize fully (for size)"),
                _T(""),
                true,

                _T(""),
                _("You have optimizations enabled. "));


    m_Options.AddOption(_("Produce debugging symbols"),
                _T("-g"),
                _("Debugging"),
                _T(""),
                true,

                _T("-O -O1 -O2 -O3 -Os"),
                _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));

    m_Options.AddOption(_("CFLAGS -mhard-div"),  // nmae
                _("-mhard-div"),  // option
                _("OpenRISC CPU architecture specific"), //category
                _(""),
                true,

                _T("You have enabled -mhard-div")); // checkMessage

    m_Options.AddOption(_("CFLAGS -mhard-mul"),_T("-mhard-mul"),_("OpenRISC CPU architecture specific"),
                        _T(""),
                        true,

                      _T(""),
                      _("You have OpenRISC CPU architecture specific"));

    m_Options.AddOption(_("Don't keep the frame pointer in a register for functions that don't need one"),
                        _T("-fomit-frame-pointer"),
                         _("OpenRISC CPU architecture specific"),
                        _T(""),
                      true,

                      _T(""),
                      _("You have OpenRISC CPU architecture specific"));
    wxString category = _("Optimization");
    m_Options.AddOption(_("Optimize more (for speed)"), _T("-O1"), category);
    m_Options.AddOption(_("Optimize even more (for speed)"), _T("-O2"), category);
    m_Options.AddOption(_("Optimize fully (for speed)"), _T("-O3"), category);

    wxString gprof_link = _T("-pg");
    if (platform::windows)
        gprof_link = _T("-pg -lgmon");
  //  m_Options.AddOption(_("Profile code when executed"), _T("-pg"), _("Profiling"), gprof_link);

     category = _("Warnings");
     m_MasterPath = _T("C:\\JH\\mc");
    // warnings
   // m_Options.AddOption(_("In C mode, support all ISO C90 programs. In C++ mode, remove GNU extensions that conflict with ISO C++"), _T("-ansi"), category);
    m_Options.AddOption(_("Enable all compiler warnings (overrides many other settings)"), _T("-Wall"), category);
    m_Options.AddOption(_("Enable extra compiler warnings"), _T("-Wextra"), category);
    m_Options.AddOption(_("Enable standard compiler warnings"), _T("-W"), category);
   // m_Options.AddOption(_("Stop compiling after first error"), _T("-Wfatal-errors"), category);
    m_Options.AddOption(_("Inhibit all warning messages"), _T("-w"), category);
//    m_Options.AddOption(_("Have g++ follow the 1998 ISO C++ language standard"), _T("-std=c++98"), category);
//    m_Options.AddOption(_("Enable warnings demanded by strict ISO C and ISO C++"), _T("-pedantic"), category);
//    m_Options.AddOption(_("Treat as errors the warnings demanded by strict ISO C and ISO C++"), _T("-pedantic-errors"), category);
    m_Options.AddOption(_("Warn if main() is not conformant"), _T("-Wmain"), category);
    m_Options.AddOption(_("Give a warning (or error) whenever a function is used before being declared"),_T("-Werror-implicit-function-declaration"),category);
//    m_Options.AddOption(_("Enable Effective-C++ warnings (thanks Scott Myers)"), _T("-Weffc++"), category);
    m_Options.AddOption(_("Warn whenever a switch statement does not have a default case"), _T("-Wswitch-default"), category);
    m_Options.AddOption(_("Warn whenever a switch statement has an index of enumerated type and lacks a case for one or more of the named codes of that enumeration"), _T("-Wswitch-enum"), category);
    m_Options.AddOption(_("Warn if a user supplied include directory does not exist"), _T("-Wmissing-include-dirs"), category);
    m_Options.AddOption(_("Warn if a global function is defined without a previous declaration"), _T("-Wmissing-declarations"), category);
    m_Options.AddOption(_("Warn if the compiler detects that code will never be executed"), _T("-Wunreachable-code"), category);
    m_Options.AddOption(_("Warn if a function can not be inlined and it was declared as inline"), _T("-Winline"), category);
    m_Options.AddOption(_("Warn if floating point values are used in equality comparisons"), _T("-Wfloat-equal"), category);
    m_Options.AddOption(_("Warn if an undefined identifier is evaluated in an '#if' directive"), _T("-Wundef"), category);
    m_Options.AddOption(_("Warn whenever a pointer is cast such that the required alignment of the target is increased"), _T("-Wcast-align"), category);
    m_Options.AddOption(_("Warn if anything is declared more than once in the same scope"), _T("-Wredundant-decls"), category);
    m_Options.AddOption(_("Warn about unitialized variables which are initialized with themselves"), _T("-Winit-self"), category);
    m_Options.AddOption(_("Warn whenever a local variable shadows another local variable, parameter or global variable or whenever a built-in function is shadowed"), _T("-Wshadow"), category);

    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Strip all symbols from binary (minimizes size)"), _T(""), category, _T("-s"), true, _T("-g -ggdb"), _("Stripping the binary will strip debugging symbols as well!"));
    m_Options.AddOption(_("Optimize generated code (for speed)"), _T("-O"), category);
//    m_Options.AddOption(_("Optimize more (for speed)"), _T("-O1"), category);
//    m_Options.AddOption(_("Optimize even more (for speed)"), _T("-O2"), category);
//    m_Options.AddOption(_("Optimize fully (for speed)"), _T("-O3"), category);

    m_Options.AddOption(_("Expensive optimizations"), _T("-fexpensive-optimizations"), category);


//    category = _("OpenRISC CPU architecture specific");

     // wxCheckListBox* clb = XRCCTRL(*this, "-mhard-div", wxCheckListBox);
      //clb->Check();
    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(_T("$compiler $options   $includes -c $file  -o $object")));
//    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(_T("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
//    m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(_T("$rescomp -i $file -J rc -o $resource_output -O coff $res_includes")));
//     wxString output = fname.GetFullPath();
//    macro.Replace(_T("$exe_output"),output);

//    m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(_T("$linker  $libdirs -o ${TARGET_OUTPUT_DIR}${PROJECT_NAME}.or32 $link_objects $link_resobjects $link_options $libs    -Tram.ld")));
     m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(_T("$linker  $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs -Map=map.txt -Tram.ld   ")));
//    if (platform::windows)
//    {
//        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(_T("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs -mwindows")));
//        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(_T("$linker -shared -Wl,--output-def=$def_output -Wl,--out-implib=$static_output -Wl,--dll $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
//    }
//    else
//    {
//        m_Commands[(int)ctLinkExeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
//        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(_T("$linker -shared $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
//    }
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(_T("$lib_linker -r -s $static_output $link_objects")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();
    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();

} // end of Reset

void CompilerOPENRISC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, _T("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("'In function...' info"), cltInfo, _T("(") + FilePathWithSpaces + _T("):[ \t]+") + _T("([iI]n ([cC]lass|[cC]onstructor|[dD]estructor|[fF]unction|[mM]ember [fF]unction).*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("'Instantiated from' info"), cltInfo, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]+([iI]nstantiated from .*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, _T("windres.exe:[ \t](") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, _T("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler note"), cltInfo, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Nn]ote:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("General note"), cltInfo, _T("([Nn]ote:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + _T("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, _T(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, _T("(") + FilePathWithSpaces + _T("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, _T("([Ww]arning:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Auto-import info"), cltInfo, _T("([Ii]nfo:[ \t].*)\\(auto-import\\)"), 1));
}

AutoDetectResult CompilerOPENRISC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
    {
        // Search for OPENRISC installation dir
         m_MasterPath = _T("C:\\JH\\mc");
         wxString sep = wxFileName::GetPathSeparator();

        if (wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C))
        {
            m_Programs.MAKE = _T("make.exe"); // we distribute "make" not "mingw32-make"
        }
    }
    else
        m_MasterPath = _T("/usr");
      AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("libc.a"));
        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("libgcc.a"));
        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("libgcov.a"));
        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("libiberty.a"));
        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("libm.a"));


    }
    return ret;
} // end of AutoDetectInstallationDir
