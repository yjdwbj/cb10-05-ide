/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5055 $
 * $Id: CompilerDSP.cpp 5055 2008-05-13 16:35:36Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/compilergcc/CompilerDSP.cpp $
 */

#ifdef __WXMSW__
// this compiler is valid only in windows

#include <sdk.h>
#include "CompilerDSP.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>



CompilerDSP::CompilerDSP()
    : Compiler(_("DSP Compiler"), _T("dsp"))

{
    Reset();

}

CompilerDSP::~CompilerDSP()
{
    //dtor
}

Compiler * CompilerDSP::CreateCopy()
{
    Compiler* c = new CompilerDSP(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerDSP::Reset()
{
    m_Programs.C = _T("g563c.exe");
    m_Programs.CPP = _T("g563c.exe");
    m_Programs.LD = _T("g563c.exe");
    m_Programs.LIB = _T("lib.exe");
    m_Programs.WINDRES = _T("");
    m_Programs.MAKE = _T("");
    m_Programs.ASM = _T("asm56300.exe");


    m_Switches.includeDirs = _T("-I");
    m_Switches.libDirs = _T("");
    m_Switches.linkLibs = _T("-L");
    m_Switches.libPrefix = _T("lib563");
    m_Switches.libExtension = _T("clb");
    m_Switches.defines = _T("-D");
    m_Switches.genericSwitch = _T("-");
    m_Switches.objectExtension = _T("cln");
    m_Switches.needDependencies = false;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = true;
    m_Switches.logging = clogSimple;
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = true;

    m_Options.ClearOptions();
//    m_Options.AddOption(_("Produce debugging symbols"),
//                _T("-g"),
//                _("Debugging"),
//                _T(""),
//                true,
//                _T("-o -o+space"),
//                _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));
//
//
//     TODO (hd#3#): should be simplified
//
//
//    . m_Options.AddOption(_("Alignment of struct members"), "-a[1|2|4|8]", _("Architecture"));
//    m_Options.AddOption(_("Enforce strict compliance with ANSI C/C++"), _T("-A"), _("C++ Features"));
//    m_Options.AddOption(_("Enable new[] and delete[] overloading"), _T("-Aa"), _("C++ Features"));
//    m_Options.AddOption(_("Enable bool"), _T("-Ab"), _("C++ Features"));
//    m_Options.AddOption(_("Enable exception handling"), _T("-Ae"), _("C++ Features"));
//    m_Options.AddOption(_("Enable RTTI"), _T("-Ar"), _("C++ Features"));
//    . m_Options.AddOption(_("Message language: English,French,German,Japanese"), _T("-B[e|f|g|j]"), _("Others"));
//    . m_Options.AddOption(_("Skip the link, do compile only"), _T("-c"), _("Others"));
//    m_Options.AddOption(_("Compile all source files as C++"), _T("-cpp"), _("C++ Features"));
//    m_Options.AddOption(_("Generate .cod (assemply) file"), _T("-cod"), _("Others"));
//    m_Options.AddOption(_("No inline function expansion"), _T("-C"), _("Debugging"));
//    m_Options.AddOption(_("Generate .dep (make dependency) file"), _T("-d"), _("Others"));
//    m_Options.AddOption(_T("#define DEBUG 1"), _T("-D"), _("Debugging"));
//    m_Options.AddOption(_("Show results of preprocessor"), _T("-e"), _("Others"));
//    m_Options.AddOption(_("Do not elide comments"), _T("-EC"), _("Others"));
//    m_Options.AddOption(_("#line directives not output"), _T("-EL"), _("Others"));
//    m_Options.AddOption(_("IEEE 754 inline 8087 code"), _T("-f"), _("Others"));
//    m_Options.AddOption(_("Work around FDIV problem"), _T("-fd"), _("Others"));
//    m_Options.AddOption(_("Fast inline 8087 code"), _T("-ff"), _("Optimization"));
//    m_Options.AddOption(_("Disable debug info optimization"), _T("-gf"), _("Optimization"));
//    m_Options.AddOption(_("Make static functions global"), _T("-gg"), _("Optimization"));
//    m_Options.AddOption(_("Symbol info for globals"), _T("-gh"), _("C++ Features"));
//    m_Options.AddOption(_("Debug line numbers only"), _T("-gl"), _("Debugging"));
//    m_Options.AddOption(_("Generate pointer validations"), _T("-gp"), _("Others"));
//    m_Options.AddOption(_("Debug symbol info only"), _T("-gs"), _("Debugging"));
//    m_Options.AddOption(_("Generate trace prolog/epilog"), _T("-gt"), _("Others"));
//    . m_Options.AddOption(_("Set data threshold to nnnn"), _T("-GTnnnn"), _("Others"));
//    m_Options.AddOption(_("Use precompiled headers (ph)"), _T("-H"), _("Others"));
//    . m_Options.AddOption(_("Use ph from directory"), _T("-HDdirectory"), _("Others"));
//    . m_Options.AddOption(_("Generate ph to filename"), _T("-HF[filename]"), _("Others"));
//    . m_Options.AddOption(_("#include \"filename\""), _T("-HIfilename"), _("Others"));
//    m_Options.AddOption(_("Include files only once"), _T("-HO"), _("Others"));
//    m_Options.AddOption(_("Only search -I directories"), _T("-HS"), _("Others"));
//    . m_Options.AddOption(_("#include file search path"), _T("-Ipath"), _("Others"));
//    . m_Options.AddOption(_("Asian language characters (Japanese)"), _T("-j0"), _("Others"));
//    . m_Options.AddOption(_("Asian language characters (Taiwanese/Chinese)"), _T("-j1"), _("Others"));
//    . m_Options.AddOption(_("Asian language characters (Korean)"), _T("-j2"), _("Others"));
//    m_Options.AddOption(_("Relaxed type checking"), _T("-Jm"), _("Others"));
//    m_Options.AddOption(_T("char==unsigned"), _T("-Ju"), _("Others"));
//    m_Options.AddOption(_("No empty base class optimization"), _T("-Jb"), _("Optimization"));
//    m_Options.AddOption(_("chars are unsigned"), _T("-J"), _("Others"));
//    . m_Options.AddOption(_("Generate list file"), _T("-l[listfile]"), _("Others"));
//    . m_Options.AddOption(_("Using non-Digital Mars linker"), _T("-L"), _("Others"));
//    . m_Options.AddOption(_("Specify linker to use"), _T("-Llink"), _("Others"));
//    . m_Options.AddOption(_("Pass /switch to linker"), _T("-L/switch"), _("Others"));
//    . m_Options.AddOption(_("Specify assembler to use"), _T("-Masm"), _("Others"));
//    . m_Options.AddOption(_("Pass /switch to assembler"), _T("-M/switch"), _("Others"));
//    . m_Options.AddOption(_("Set memory model (-mn: Windows)"), _T("-m[tsmclvfnrpxz][do][w][u]"), _("Architecture"));
//    m_Options.AddOption(_("Perform function level linking"), _T("-Nc"), _("Optimization"));
//    m_Options.AddOption(_("No default library"), _T("-NL"), _("Optimization"));
//    m_Options.AddOption(_("Place expr strings in code seg"), _T("-Ns"), _("Optimization"));
//    m_Options.AddOption(_("New code seg for each function"), _T("-NS"), _("Optimization"));
//    . m_Options.AddOption(_("Set code segment name"), _T("-NTname"), _("Others"));
//    m_Options.AddOption(_("vtables in far data"), _T("-NV"), _("Others"));
//    . m_Options.AddOption(_("Run optimizer with flag"), _T("-o[-+flag]"), _("Optimization"));
//    m_Options.AddOption(_("Minimize space"), _T("-o+space"), _("Optimization"));
//    m_Options.AddOption(_("Maximize speed"), _T("-o"), _("Optimization"));
//    . m_Options.AddOption(_("Output filename"), _T("-ooutput"), _("Others"));
//    m_Options.AddOption(_("Turn off function auto-prototyping"), _T("-p"), _("Others"));
//    m_Options.AddOption(_("Make Pascal linkage the default"), _T("-P"), _("Linkage"));
//    m_Options.AddOption(_("Make stdcall linkage the default"), _T("-Pz"), _("Linkage"));
//    m_Options.AddOption(_("Require strict function prototyping"), _T("-r"), _("Others"));
//    m_Options.AddOption(_("Put switch tables in code seg"), _T("-R"), _("Others"));
//    m_Options.AddOption(_("Stack overflow checking"), _T("-s"), _("Others"));
//    m_Options.AddOption(_("Always generate stack frame"), _T("-S"), _("Others"));
//    m_Options.AddOption(_("Suppress non-ANSI predefined macros"), _T("-u"), _("C++ Features"));
//    . m_Options.AddOption(_("Verbose compile"), _T("-v[0|1|2]"), _("Warnings"));
//    m_Options.AddOption(_("Suppress all warning messages"), _T("-w"), _("Warnings"));
//    m_Options.AddOption(_("Warn on C style casts"), _T("-wc"), _("Warnings"));
//    . m_Options.AddOption(_("Suppress warning number n"), _T("-wn"), _("Warnings"));
//    m_Options.AddOption(_("Treat warnings as errors"), _T("-wx"), _("Warnings"));
//    . m_Options.AddOption(_("Windows prolog/epilog (-WA exe -WD dll)"), _T("-W{0123ADabdefmrstuvwx-+}"), _("Architecture"));
//    m_Options.AddOption(_("Windows prolog/epilog : Win32 Exe"), _T("-WA"), _("Architecture"));
//    m_Options.AddOption(_("Windows prolog/epilog : Win32 Dll"), _T("-WD"), _("Architecture"));
//    m_Options.AddOption(_("Turn off error maximum"), _T("-x"), _("Warnings"));
//    m_Options.AddOption(_("Instantiate templates"), _T("-XD"), _("C++ Features"));
//    . m_Options.AddOption(_("Instantiate template class temp<type>"), _T("-XItemp<type>"), _("C++ Features"));
//    . m_Options.AddOption(_("Instantiate template function func(type)"), _T("-XIfunc(type)"), _("C++ Features"));
//    . m_Options.AddOption(_("8088/286/386/486/Pentium/P6 code"), _T("-[0|2|3|4|5|6]"), _("Architecture"));
//    m_Options.AddOption(_("Optimize for 80386"), _T("-3"), _("Architecture"));
//    m_Options.AddOption(_("Optimize for 80486"), _T("-4"), _("Architecture"));
//    m_Options.AddOption(_("Optimize for Pentium"), _T("-5"), _("Architecture"));
//    m_Options.AddOption(_("Optimize for Pentium Pro, Pentium II, Pentium III"), _T("-6"), _("Architecture"));

    // FIXME (hd#1#): should be work on: we need $res_options
    wxString category = _("Optimization");
     m_Options.AddOption(_("Produce COFF debugging information."), _T("-g"), category);
    m_Options.AddOption(_("Pass the argument string directly to asm56300, the DSP56300 assembler."), _T("-asm"), category);
    m_Options.AddOption(_("Compile and/or assemble the source ﬁles."), _T("-c"), category);

    m_Options.AddOption(_("Run the assembly language optimizer on the assembly language output of\
g563-cc1. "), _T("-alo"), category);
    m_Options.AddOption(_("Disable all optimizations."), _T("-fno-opt"), category);
    m_Options.AddOption(_("Disable the peephole portion of optimization."), _T("-fno-peephole"), category);
    m_Options.AddOption(_("Disable the optimizations of loop strength "), _T("-fno-strength-reduce"), category);
    m_Options.AddOption(_("By default, the compiler will try to defer (delay) restoring the stack pointer upon the\
return of a function call. "), _T("-fno-defer-pop"), category);
    m_Options.AddOption(_("-fforce-addr"), _T("-fforce-addr"), category);
    m_Options.AddOption(_("-finline-functions"), _T("-finline-functions"), category);
    m_Options.AddOption(_("-fcaller-saves"), _T("-fcaller-saves"), category);
    m_Options.AddOption(_("-fkeep-inline-functions"), _T("-fkeep-inline-functions"), category);
    m_Options.AddOption(_("-fwritable-strings"), _T("-fwritable-strings"), category);
    m_Options.AddOption(_("-fcond-mismatch"), _T("-fcond-mismatch"), category);
    m_Options.AddOption(_("-fvolatile"), _T("-fvolatile"), category);
    m_Options.AddOption(_("fixed register r0"), _T("-ffixed-r0"), category);
    m_Options.AddOption(_("fixed register r1"), _T("-ffixed-r1"), category);
    m_Options.AddOption(_("fixed register r2"), _T("-ffixed-r2"), category);
    m_Options.AddOption(_("fixed register r3"), _T("-ffixed-r4"), category);
    m_Options.AddOption(_("fixed register r4"), _T("-ffixed-r5"), category);

    m_Options.AddOption(_("Perform machine dependent and independent optimizations. This is the default\
mode of the compiler.."), _T("-O"), category);
    m_Options.AddOption(_("Generate code that consumes less program memory at the expense of run time."), _T("-mconserve-p-mem"), category);
    m_Options.AddOption(_("Disables all Motorola optimizer enhancements."), _T("-mno-dsp-optimization"), category);
    m_Options.AddOption(_("Disable DO instruction usage by optimizer."), _T("-mno-do-loop-generation"), category);
    m_Options.AddOption(_("Disable the promotion of address expressions to address registers within loops"), _T("-mno-biv-plus-linv-promotion"), category);
    m_Options.AddOption(_("Forces the compiler to locate all switch tables in P memory."), _T("-mp-mem-switchtable"), category);
    m_Options.AddOption(_("Direct the compiler to locate data in the X data memory space."), _T("-mx-memory"), category);
    m_Options.AddOption(_("Direct the compiler to locate data in the Y data memory space. "), _T("-my-memory"), category);
    m_Options.AddOption(_("Direct the compiler to locate data in the L data memory space. "), _T("-ml-memory"), category);
    m_Options.AddOption(_("Generate extra run-time code to check for run-time stack collision with the heap."), _T("-mstack-check"), category);
    m_Options.AddOption(_("Issue all the warnings demanded by strict ANSI standard C"), _T("-pedantic"), category);
    m_Options.AddOption(_("Direct the compiler to execute in verbose mode."), _T("-Q"), category);
    m_Options.AddOption(_("Compile to DSP56300 assembly code with the original C source lines as comments\
but do not assemble"), _T("-S"), category);
    m_Options.AddOption(_("Inhibit all warning messages."), _T("-w"), category);
    m_Options.AddOption(_("Compile and/or assemble the source ﬁles, suppressing the link phase."), _T("-c"), category);


    m_Commands[0].push_back( CompilerTool(_T("$compiler $includes -B$libdirs   $file  -g -c  -S -asm \" -l\"   ")));
    //m_Commands[4].push_back( CompilerTool(_T("$linker -EA errors   -m -Bdspmain.cld -rdsp.ctl  $link_objects ")));
    m_Commands[4].push_back( CompilerTool(_T("$linker $includes -B$libdirs  #allfile -j \" -m -rex8.ctl\"   $options -o dspmain.cld    ")));

   // m_Commands[(int)ctLinkDynamicCmd].push_back( CompilerTool(_T("$linker /NOLOGO /subsystem:windows $link_objects, $exe_output, , $libs $link_options, , $link_resobjects")) );
   // m_Commands[(int)ctLinkStaticCmd].push_back( CompilerTool(_T("$lib_linker -c $link_options $static_output $link_objects")) );
//    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerDSP::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, _T("(") + FilePathWithSpaces + _T(")[ \t]+:[ \t]+(.*error LNK[0-9]+.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, _T("(") + FilePathWithSpaces + _T(")\\(([0-9]+)\\)[ \t]*:[ \t]*[Ww][Aa][Rr][Nn][Ii][Nn][Gg][ \t]*(.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, _T("(") + FilePathWithSpaces + _T(")\\(([0-9]+)\\)[ \t]*:[ \t]*(.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, _T("Fatal error:[ \t](.*)"), 1));
}

AutoDetectResult CompilerDSP::AutoDetectInstallationDir()
{
    // just a guess; the default installation dir
    m_MasterPath = _T("C:\\JH\\dsp5xx");
    wxString sep = wxFileName::GetPathSeparator();

    // NOTE (hd#1#): dmc uses sc.ini for compiler's master directories
    // NOTE (mandrav#1#): which doesn't seem to exist if you don't have the CD version ;)
    if (!m_MasterPath.IsEmpty())
    {
       // AddIncludeDir(m_MasterPath + sep + _T("stlport") + sep + _T("stlport"));
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));


    }

   // return wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
     AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + sep + _T("include"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
//        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("crt0563l.cln"));
//        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("crt0563x.cln"));
//        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("crt0563y.cln"));
//        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("lib563cl.clb"));
//        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("lib563cx.clb"));
//        AddLinkLib(m_MasterPath + sep + _T("lib")+ sep + _T("lib563cy.clb"));


    }
    return ret;
     //return wxFileExists(m_MasterPath  + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
