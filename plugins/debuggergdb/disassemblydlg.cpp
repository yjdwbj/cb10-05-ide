/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5215 $
 * $Id: disassemblydlg.cpp 5215 2008-09-24 07:15:22Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/debuggergdb/disassemblydlg.cpp $
 */

#include <sdk.h>
#include "disassemblydlg.h"
#include "debuggergdb.h"

#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/wfstream.h>
#include <wx/fontutil.h>
#include <globals.h>
#include <editorcolourset.h>
#include <manager.h>
#include <editormanager.h>
#include <configmanager.h>
#include <projectmanager.h>
#include <cbproject.h>
#include <filefilters.h>

#include "disassemblytextctrl.h"
#include <wx/stopwatch.h>
#include <wx/textfile.h>
#define DEBUG_MARKER        2
#define DEBUG_STYLE         wxSCI_MARGIN_NUMBER
//#define STYLE_LINENUMBER    33
//#define DEBUG_STYLE         wxSCI_MARK_ARROW

static int findnum = -1;
int n =0;
static int m_LineCount=0;
int idSCI_Dclick = wxNewId();
static wxRegEx reRelativePath(_T("(.*):([0-9]+)"));
static wxRegEx reInvaildPath(_T("^\\/(.*)\\/$"));
BEGIN_EVENT_TABLE(DisassemblyDlg, wxPanel)
    EVT_BUTTON(XRCID("btnSave"), DisassemblyDlg::OnSave)
    EVT_BUTTON(XRCID("btnFindNext"), DisassemblyDlg::OnFind)

//    EVT_BUTTON(XRCID("btnRefresh"), DisassemblyDlg::OnRefresh)
END_EVENT_TABLE()
DisassemblyDlg::DisassemblyDlg(wxWindow* parent, DebuggerGDB* debugger)
    : m_pDbg(debugger),
    m_LastActiveAddr(0),
    m_ClearFlag(false),
    m_IntermixLine(0)
{
    //ctor
    wxXmlResource::Get()->LoadPanel(this, parent, _T("dlgDisassembly"));
//    SetWindowStyle(GetWindowStyle() | wxFRAME_FLOAT_ON_PARENT);

    m_pCode = new DisassemblyTextCtrl(this, wxID_ANY);
    m_pCode->SetReadOnly(true);
    m_pCode->SetCaretWidth(0);
    m_pCode->SetMarginWidth(0, 0);
    m_pCode->SetMarginType(1, wxSCI_MARGIN_SYMBOL);
    m_pCode->SetMarginSensitive(1, 1);
    m_pCode->SetMarginMask(1, (1 << DEBUG_MARKER));
    m_pCode->MarkerDefine(DEBUG_MARKER, DEBUG_STYLE);
    m_pCode->MarkerSetBackground(DEBUG_MARKER, wxColour(0xFF, 0x0, 0x0));

    wxXmlResource::Get()->AttachUnknownControl(_T("lcCode"), m_pCode);

    // use the same font as editor's
    wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
    wxString fontstring = Manager::Get()->GetConfigManager(_T("editor"))->Read(_T("/font"), wxEmptyString);
    if (!fontstring.IsEmpty())
    {
        wxNativeFontInfo nfi;
        nfi.FromString(fontstring);
        font.SetNativeFontInfo(nfi);
    }

    m_pCode->StyleSetFont(wxSCI_STYLE_DEFAULT, font);

    EditorColourSet* set = Manager::Get()->GetEditorManager()->GetColourSet();
    if (set)
    {
        HighlightLanguage lang = set->GetHighlightLanguage(wxSCI_LEX_ASM);
        set->Apply(lang, (cbStyledTextCtrl*)m_pCode);
    }

    StackFrame sf;
    Clear(sf);
}


DisassemblyDlg::~DisassemblyDlg()
{
    //dtor
}

void DisassemblyDlg::Clear(const StackFrame& frame)
{
    m_FrameFunction = frame.valid ? frame.function : _T("??");
    //m_Frame = m_pDbg->m_pDisassembly;
    m_FrameAddress = _T("??");
    if (frame.valid)
        m_FrameAddress.Printf(_T("%p"), (void*)frame.address);

    XRCCTRL(*this, "lblFunction", wxStaticText)->SetLabel(m_FrameFunction);
    XRCCTRL(*this, "lblAddress", wxStaticText)->SetLabel(m_FrameAddress);


    m_HasActiveAddr = false;

    m_pCode->SetReadOnly(false);
    if (frame.valid && m_pDbg->IsRunning())  // lcy disassembling
    {
        // if debugger is running, show a message
        m_pCode->SetText(_("\"Please wait while disassembling...\""));
        m_ClearFlag = true; // clear the above message when adding the first line
    }
    else
    {
        // if debugger isn't running, just clear the window
        m_pCode->ClearAll();
        m_ClearFlag = false;
    }
    m_pCode->SetReadOnly(true);
    m_pCode->MarkerDeleteAll(DEBUG_MARKER);
}


void DisassemblyDlg::SetAssemblerLine(unsigned long int addr)
{
    m_pCode->SetReadOnly(false);
    if(m_ClearFlag)
    {
        m_ClearFlag = false;
        m_pCode->ClearAll();
    }
    if (m_HasActiveAddr && addr == m_LastActiveAddr)
        return;
    m_HasActiveAddr = false;
    m_LastActiveAddr = addr;
    unsigned long int  num = m_LastActiveAddr ;

    if(m_LastActiveAddr > addr)
    {
        num = num - (m_LastActiveAddr - addr);
    }
    else
    {
        num = num + (addr - m_LastActiveAddr  );
    }

   // SetActiveAddress(m_LastActiveAddr);

            m_pCode->MarkerDeleteAll(DEBUG_MARKER);
            m_pCode->MarkerAdd( num,DEBUG_MARKER);
            m_pCode->SetSelectionMode(1);
            m_pCode->SetFirstVisibleLine(num);
            wxString atmp = m_pCode->GetLine(num);

            int CodeLen= m_pCode->GetLength();
            int linelen = atmp.Length();
             ;
            // m_pCode->SetAnchor(num);
            m_pCode->GotoLine(num);
             m_pCode->AddSelection(0,0);

           // m_pCode->ScrollToLine(num);
            m_LastActiveAddr = num;
            m_HasActiveAddr = true;

    m_pCode->SetReadOnly(true);
}


void DisassemblyDlg::SetIntermixAssemblerLine(/*unsigned long int addr const wxString& cmp */)
{

//     if(m_LineCount < m_IntermixLine.GetCount())
//     {
//        ProcessIntermixLine(m_IntermixLine.GetCount());
//     }

     int i;
     m_pCode->SetReadOnly(false);

            wxString tmpLine,tcutaddr;
            wxString tcmp=wxString::Format(_("%x"),m_LastActiveAddr) << _T(':');

           for(i =0;i < m_pCode->GetLineCount();i++)
           {
               if(m_pCode->GetLine(i).Contains(tcmp))
               {
                   break;
               }
           }

          SetMarkLine(i);
         SetActiveAddress(m_LastActiveAddr);
}

void DisassemblyDlg::SetMarkLine(const int i)
{


    m_SelectionLine.Printf(_T("%d"),i);
    XRCCTRL(*this, "labSelectionLine", wxStaticText)->SetLabel(m_SelectionLine);
    m_pCode->SetReadOnly(false);
    m_pCode->MarkerDeleteAll(DEBUG_MARKER);
    m_pCode->MarkerAdd(i,DEBUG_MARKER);
    m_pCode->SetFirstVisibleLine(i-5);
    m_pCode->GotoLine(i);
    //m_pCode->EnsureVisible(i);
    m_pCode->SetSelectionStart(m_pCode->GetSelectionStart());
    m_pCode->SetSelectionEnd(m_pCode->GetLineEndPosition(i));

    m_pCode->SetReadOnly(true);
}

void DisassemblyDlg::ProcessIntermixLine(int end)
{
    m_pCode->SetReadOnly(false);

      // wxString prjname = Manager::Get()->GetProjectManager()->GetActiveProject()->GetTitle();
      //Manager::Get()->GetProjectManager()->GetActiveProject()->Get
       wxString prjpath,basepath;
       prjpath = Manager::Get()->GetProjectManager()->GetActiveProject()->GetExecutionDir();
       basepath = prjpath;
       prjpath.RemoveLast(1);
       prjpath=prjpath.AfterLast(_T('\\'));
//       t2 = Manager::Get()->GetProjectManager()->GetActiveProject()->GetWorkingDir();
//       t3 = Manager::Get()->GetProjectManager()->GetActiveProject()->GetBasePath();
       int i = m_LineCount;
        for(i ;i < end;i++)
        {
            wxString tmpline=m_IntermixLine[i];
            if(tmpline.StartsWith(_("Disassembly")) || tmpline.StartsWith(_("main.or32"))
                                || tmpline.IsEmpty() || tmpline.Contains(_("..."))
                                ||reInvaildPath.Matches(tmpline) || tmpline.StartsWith(_("__"))
                                || tmpline.Contains(_T("elf32-or32")))
            {
               continue;
            }

            if(tmpline.Contains(_T("/cygdrive/")))
            {
                tmpline.Remove(0,10);
                wxString t = tmpline.BeforeFirst(_T('/')) << _T(":/");
                t.UpperCase();
                t << tmpline.AfterFirst(_T('/'));
                tmpline = t;
                tmpline.Replace(_T("/"),_T("\\"));
                if(tmpline.Contains(basepath));
                tmpline.Remove(0,basepath.Length());
            }
            else if(tmpline.Contains(_("\\")))
            {
                wxString t  = tmpline.BeforeFirst(_T('\\'));
                t = t.AfterLast(_T('/'));
                t << _T("\\") <<tmpline.AfterFirst(_T('\\'));
                tmpline = t;
            }
//            if(tmpline.Contains(prjpath))
//            {
//                while(tmpline.Contains(prjpath))
//               tmpline = tmpline.Mid(tmpline.Find(prjpath)+prjpath.Length()+1);
//            }
            else if(reRelativePath.Matches(tmpline))
            {
                if(tmpline.Contains(_T('/')))
                tmpline=tmpline.AfterFirst(_T('/'));
                tmpline.Replace(_T("/"),_T("\\"));
            }

            if(tmpline.IsEmpty())
            continue;
            m_pCode->AppendText(tmpline +_T('\n'));
        }
        m_LineCount=i;
         m_pCode->SetReadOnly(true);
}


void DisassemblyDlg::AddIntermixAssemblyLine()
{

       m_pCode->ClearAll();
       m_LineCount=0;
       m_pCode->SetStatus(1);
       m_IntermixLine.Empty();
//       wxString compilerid = Manager::Get()->GetProjectManager()->GetActiveProject()->GetCompilerID();
//       wxString  objdump = CompilerFactory::GetCompiler(compilerid)->GetPrograms().OBJDUMP + _("  -S main.or32 -l ");
//       wxExecute(objdump, m_IntermixLine, wxEXEC_ASYNC);

            wxTextFile dump(_T("intermix.lit"));
            dump.Open(wxConvFile);
            wxString code;
            for(code=dump.GetFirstLine();!dump.Eof();code=dump.GetNextLine())
            {
                m_IntermixLine.Add(code);
            }
            m_IntermixLine.Add(_T("\r\n"));
            dump.Close();
       ProcessIntermixLine(m_IntermixLine.GetCount());
    SetActiveAddress(m_LastActiveAddr);

    if(m_IntermixLine.IsEmpty())
    {
        wxMessageBox(_T("Disassembly area is empty , Please Rebuild the target and then do it again!"));
    }
}


void DisassemblyDlg::AddAssemblerLine(unsigned long int addr, const wxString& line)
{
    m_pCode->SetReadOnly(false);

    if (m_ClearFlag)
    {
        m_ClearFlag = false;
        m_pCode->ClearAll();
    }
    wxString fmt;
    fmt.Printf(_T("%p\t%s\n"), (void*)addr, line.c_str());
    m_pCode->AppendText(fmt);

    SetActiveAddress(m_LastActiveAddr);

    m_pCode->SetReadOnly(true);
}

void DisassemblyDlg::SetActiveAddress(unsigned long int addr)
{
    if (m_HasActiveAddr && addr == m_LastActiveAddr)
        return;
    m_HasActiveAddr = false;
    m_LastActiveAddr = addr;


    for (int i = 0; i < m_pCode->GetLineCount(); ++i)
    {
        int wlinec=m_pCode->GetLineCount();
        wxString wline=m_pCode->GetLine(i);
        wxString str;
        wxString compilerid = Manager::Get()->GetProjectManager()->GetActiveProject()->GetCompilerID();
        Compiler* a = CompilerFactory::GetCompiler(compilerid);
        if(a->GetParentID().Matches(_T("lcy")))
             str = m_pCode->GetLine(i).BeforeFirst(_T(':'));
        else
            str = m_pCode->GetLine(i).AfterFirst(_T('x')).BeforeFirst(_T('\t'));
        unsigned long int lineaddr;

        if (str.ToULong(&lineaddr, 16) && lineaddr >= addr)
        {
            m_pCode->MarkerDeleteAll(DEBUG_MARKER);
            m_pCode->MarkerAdd(i, DEBUG_MARKER);
            m_pCode->GotoLine(i);
            m_pCode->EnsureVisible(i);
            m_HasActiveAddr = true;
            break;
        }

    }
}

void DisassemblyDlg::OnFind(wxCommandEvent& event)
{

    if(m_LineCount < m_IntermixLine.GetCount())
    ProcessIntermixLine(m_IntermixLine.GetCount());
     wxString FindStr(XRCCTRL(*this, "cboxFindStr", wxComboBox)->GetValue());
     findnum++;
   //  int i = !m_pCode->GetFirstVisibleLine() ? m_pCode->GetFirstVisibleLine() : m_pCode->GetFirstVisibleLine()+6;
     for(; findnum <= m_IntermixLine.GetCount();findnum++)
     {
         if(m_pCode->GetLine(findnum).Contains(FindStr))
              break;
         else
            continue;
     }

    if(findnum <= m_IntermixLine.GetCount())
    {
        if(m_pCode->GetLine(findnum).Contains(FindStr))
        {

            SetMarkLine(findnum);
        }
    }
    else
    {
        findnum = -1;
    }
//     if(findnum!=m_IntermixLine.GetCount())
//     SetMarkLine(findnum);

}


void DisassemblyDlg::OnSave(wxCommandEvent& event)
{
    wxFileDialog dlg(this,
                     _("Save as text file"),
                     _T("assembly_dump.txt"),
                     wxEmptyString,
                     FileFilters::GetFilterAll(),
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;

    wxString output;
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (prj)
    {
        output << _("Project title : ") << prj->GetTitle() << _T('\n');
        output << _("Project path  : ") << prj->GetBasePath() << _T('\n') << _T('\n');
    }

    output << _("Frame function: ") << m_FrameFunction << _T('\n');
    output << _("Frame address : ") << m_FrameAddress << _T('\n');
    output << wxString(_T('-'), 80) << _T('\n');
    output << m_pCode->GetText();

    if (!cbSaveToFile(dlg.GetPath(), output))
        cbMessageBox(_("Could not save file..."), _("Error"), wxICON_ERROR);
}

void DisassemblyDlg::OnRefresh(wxCommandEvent& event)
{
    m_pDbg->Disassemble();
}


