/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef EXAMINEMEMORYDLG_H
#define EXAMINEMEMORYDLG_H

#include <wx/panel.h>

class DebuggerGDB;
class wxTextCtrl;

class ExamineMemoryDlg : public wxPanel
{
    public:
        ExamineMemoryDlg(wxWindow* parent, DebuggerGDB* debugger);
        virtual ~ExamineMemoryDlg();

        // used for Freeze()/Thaw() calls
        void Begin();
        void End();

        void Clear();
        wxString GetBaseAddress();
        int GetBytes();
        void AddError(const wxString& err);
        void AddHexByte(const wxString& addr, const wxString& hexbyte);
        void AddHexString(const wxString& addr);
    protected:
        void OnGo(wxCommandEvent& event);
        DebuggerGDB* m_pDbg;
        wxTextCtrl* m_pText;
        size_t m_ByteCounter;
        wxChar m_LineText[54]; // 16*3 "7F " + 3 "   " + 16 "."
        /// 2012-9-14 lcy chanage 9*4 "11223344 " + 3 " " + 16 "." == 54
        unsigned long m_LastRowStartingAddress;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // EXAMINEMEMORYDLG_H
