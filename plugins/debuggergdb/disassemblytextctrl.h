/*
 * This file is part of the MC::Tools IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef DISASSEMBLYTEXTCTRL_H_INCLUDED
#define DISASSEMBLYTEXTCTRL_H_INCLUDED

#include "wx/wxscintilla.h"
#include <wx/longlong.h>

class wxContextMenuEvent;
class wxMouseEvent;
class cbProject;
class DebuggerGDB;



class DisassemblyTextCtrl : public wxScintilla
{
    public:
        DisassemblyTextCtrl(wxWindow* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~DisassemblyTextCtrl();
        wxDateTime GetLastFocusTime() const {return m_lastFocusTime;}
    private:
       // void OnContextMenu(wxContextMenuEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnGetFocus(wxFocusEvent& event);
        void OnGPM(wxMouseEvent& event);
        void OnDclick(wxMouseEvent& event);
        void SyncEditor(const wxString& filename, int line, bool setMarker);
        void ClearActiveMarkFromAllEditors();

        DebuggerGDB* m_pDbg;
        wxWindow* m_pParent;
        wxLongLong m_lastFocusTime;

        DECLARE_EVENT_TABLE()
};

#endif // CBSTYLEDTEXTCTRL_H_INCLUDED
