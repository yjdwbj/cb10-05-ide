/*
 * This file is part of the MC::Tools IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

//#include "sdk_precomp.h"
//#ifndef CB_PRECOMP
//    #include "globals.h"
//#endif

#include <wx/string.h>
#include <wx/timer.h>
#include <wx/regex.h>
#include <manager.h>
#include <configmanager.h>
#include <logmanager.h>
#include <projectmanager.h>
#include <pluginmanager.h>
#include <editormanager.h>
#include <macrosmanager.h>
#include <cbeditor.h>
#include <projectbuildtarget.h>
#include <projectloader_hooks.h>
#include <cbproject.h>

#include "disassemblytextctrl.h"
#include "editorbase.h" // DisplayContextMenu
#include "prep.h" // platform::gtk
#include "debuggergdb.h"

#define DEBUG_MARKER        4
#define DEBUG_STYLE         wxSCI_MARK_ARROW
//static wxRegEx reFileNumber(_T("^/(cygdrive)/([0-9a-z./_-]+):([0-9]+)"));
//   /cygdrive/e/t1/dgdgggdgmc/main.c:14
//static wxRegEx reFullPath(_T("^[a-zA-Z]:(.*):([0-9]+)"));
static wxRegEx reRelativePath(_T("(.*):([0-9]+)"));
static bool once=1;
BEGIN_EVENT_TABLE(DisassemblyTextCtrl, wxScintilla)
   // EVT_CONTEXT_MENU(DisassemblyTextCtrl::OnContextMenu)
    //EVT_KILL_FOCUS(DisassemblyTextCtrl::OnKillFocus)
    //EVT_MIDDLE_DOWN(DisassemblyTextCtrl::OnGPM)
    //EVT_SET_FOCUS(DisassemblyTextCtrl::OnGetFocus)
    EVT_LEFT_DOWN(DisassemblyTextCtrl::OnDclick)
END_EVENT_TABLE()

DisassemblyTextCtrl::DisassemblyTextCtrl(wxWindow* pParent, int id, const wxPoint& pos, const wxSize& size, long style)
    : wxScintilla(pParent, id, pos, size, style),
    m_pParent(pParent),
    m_lastFocusTime(0L)
{
   //ctor
}

DisassemblyTextCtrl::~DisassemblyTextCtrl()
{
    //dtor

}

// events

void DisassemblyTextCtrl::OnKillFocus(wxFocusEvent& event)
{
    // cancel auto-completion list when losing focus
    if (AutoCompActive())
    {
        AutoCompCancel();
    }
    if (CallTipActive())
    {
        CallTipCancel();
    }
    event.Skip();
} // end of OnKillFocus

void DisassemblyTextCtrl::OnGetFocus(wxFocusEvent& event)
{
    // store timestamp for use in cbEditor::GetControl()
    // don't use event.GetTimeStamp(), because the focus event has no timestamp !
    m_lastFocusTime = wxGetLocalTimeMillis();
    event.Skip();
} // end of OnGetFocus

//void DisassemblyTextCtrl::OnContextMenu(wxContextMenuEvent& event)
//{
//    if ( m_pParent )
//    {
//        if ( EditorBase* pParent = dynamic_cast<EditorBase*>(m_pParent) )
//        {
//            const bool is_right_click = event.GetPosition() != wxDefaultPosition;
//            const wxPoint mp(is_right_click ? event.GetPosition() : wxDefaultPosition);
//            pParent->DisplayContextMenu(mp, mtEditorManager);
//        }
//        else
//        {
//            event.Skip();
//        }
//    }
//} // end of OnContextMenu

void DisassemblyTextCtrl::OnGPM(wxMouseEvent& event)
{
    if(platform::gtk == false) // only if GPM is not already implemented by the OS
    {
        int pos = PositionFromPoint(wxPoint(event.GetX(), event.GetY()));

        if(pos == wxSCI_INVALID_POSITION)
            return;

        int start = GetSelectionStart();
        int end = GetSelectionEnd();

        const wxString s = GetSelectedText();

        if(pos < GetCurrentPos())
        {
            start += s.length();
            end += s.length();
        }

        InsertText(pos, s);
        SetSelectionVoid(start, end);
    }
} // end of OnGPM

void DisassemblyTextCtrl::OnDclick(wxMouseEvent& event)
{

        if(GetStatus())
        {
            SetStatus(0);
           PluginsArray plugins = Manager::Get()->GetPluginManager()->GetDebuggerOffers();
            if (plugins.GetCount())
            {
                cbDebuggerPlugin* dbg = (cbDebuggerPlugin*)plugins[0];
                if (dbg)
                {
                    // is the debugger running?
                    if (dbg->IsRunning())
                    {
                        dbg->StepByStep();
                    }
                }
            }
        }
        int LineNum = GetCurrentLine();
        int L2 = GetCurrentLine();
        wxString LineText,SourceFile;
       unsigned long SourceLine=0;

        while(LineNum > 0)
        {
            LineText = GetLine(LineNum--);
            if(reRelativePath.Matches(LineText))
            break;
        }

        if(LineText.IsEmpty())
        return ;

        LineText.AfterLast(_T(':')).ToULong(&SourceLine,10);
        SourceFile = Manager::Get()->GetProjectManager()->GetActiveProject()->GetBasePath();
        SourceFile  <<  LineText.Before(_T(':'));



        SyncEditor(LineText.Before(_T(':')),SourceLine,true);

        SetReadOnly(false);
        MarkerDeleteAll(DEBUG_MARKER);
        MarkerAdd(SourceLine,DEBUG_MARKER);


        SetReadOnly(true);
    //wxMessageBox(wxString::Format(_T("%s:%d"),SourceFile.c_str(),SourceLine));
     event.Skip();

}

void DisassemblyTextCtrl::SyncEditor(const wxString& filename, int line, bool setMarker) ///
{
    if (setMarker)
        ClearActiveMarkFromAllEditors();
   // wxString testfile(filename);
    FileType ft = FileTypeOf(filename);
    if (ft != ftSource && ft != ftHeader && ft != ftResource)
        return; // don't try to open unknown files
    cbProject* project = Manager::Get()->GetProjectManager()->GetActiveProject();
    ProjectFile* f = project ? project->GetFileByFilename(filename, false, true) : 0;
    wxFileName fname(filename);
    if (project && fname.IsRelative())
        fname.MakeAbsolute(project->GetBasePath());
    // gdb can't work with spaces in filenames, so we have passed it the shorthand form (C:\MYDOCU~1 etc)
    // revert this change now so the file can be located and opened...
    // we do this by calling GetLongPath()
    cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetLongPath());
    if (ed)
    {
        ed->Show(true);
        if (f && !ed->GetProjectFile())
            ed->SetProjectFile(f);
        ed->GotoLine(line - 1, false);
        if (setMarker)
        {
            ed->SetDebugLine(line - 1);
            ed->SetFirstVisibleLineA(line-5);
        }

    }
//    else
//        Log(_("Cannot open file: ") + fname.GetLongPath());
}

void DisassemblyTextCtrl::ClearActiveMarkFromAllEditors()
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
//  Plugins are destroyed prior to EditorManager, so this is guaranteed to be valid at all times
//    if (!edMan)
//        return;
    for (int i = 0; i < edMan->GetEditorsCount(); ++i)
    {
        cbEditor* ed = edMan->GetBuiltinEditor(i);
        if (ed)
            ed->SetDebugLine(-1);
    }
}


