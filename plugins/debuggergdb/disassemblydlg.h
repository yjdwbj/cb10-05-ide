/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef DISASSEMBLYDLG_H
#define DISASSEMBLYDLG_H

#include <wx/panel.h>
#include <sdk_events.h>

class DebuggerGDB;
class DisassemblyTextCtrl;
class cbEditor;
class wxScintillaEvent;
struct StackFrame;


class DisassemblyDlg : public wxPanel
{
    public:
        DisassemblyDlg(wxWindow* parent, DebuggerGDB* debugger);
        virtual ~DisassemblyDlg();

        void Clear(const StackFrame& frame);
        void AddAssemblerLine(unsigned long int addr, const wxString& line);
        void AddIntermixAssemblyLine();
        void SetAssemblerLine(unsigned long int addr);
        void SetIntermixAssemblerLine(/*unsigned long int addrconst wxString& cmp */);
        void SetActiveAddress(unsigned long int addr);
        unsigned long int GetActiveAddress() {return m_LastActiveAddr ;}
//        unsigned int GetIntermixLineCount() {return m_IntermixLine.GetCount();}
        wxArrayString& GetIntermixLine() {return m_IntermixLine;}
        void SetMarkLine(const int line);
        void ProcessIntermixLine(int end);


    protected:
        void OnSave(wxCommandEvent& event);
        void OnFind(wxCommandEvent& event);
        void OnRefresh(wxCommandEvent& event);
        DebuggerGDB* m_pDbg;
        DisassemblyTextCtrl* m_pCode;
        unsigned long int m_LastActiveAddr;
        bool m_HasActiveAddr;
        wxString m_FrameFunction;
        wxString m_FrameAddress;
        wxString m_SelectionLine;
        wxArrayString m_IntermixLine;

        //StackFrame& m_Frame;
        bool m_ClearFlag;

    private:

        DECLARE_EVENT_TABLE();
};

#endif // DISASSEMBLYDLG_H
