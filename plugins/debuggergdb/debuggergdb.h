/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef DEBUGGERGDB_H
#define DEBUGGERGDB_H

#include <map>

#include <settings.h> // much of the SDK is here
#include <sdk_events.h>
#include <cbplugin.h>
#include <loggers.h>
#include <pipedprocess.h>
#include <wx/regex.h>
#include <wx/event.h>
#include <editor_hooks.h>
#include "remotedebugging.h"
#include "debuggerstate.h"
#include "debugger_defs.h"
#include "backtracedlg.h"
#include "disassemblydlg.h"
#include "cpuregistersdlg.h"
#include "breakpointsdlg.h"
#include "threadsdlg.h"
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/socket.h>

#define MAX_JTAG 2


extern const wxString g_EscapeChar;

class cbProject;
class TiXmlElement;
class DebuggerDriver;
class DebuggerCmd;
class Compiler;

class DebuggerTree;
class DisassemblyDlg;
class CPURegistersDlg;
class BacktraceDlg;
class BreakpointsDlg;
class ExamineMemoryDlg;
class ThreadsDlg;



class DebugTextCtrlLogger;

class DebuggerGDB : public cbDebuggerPlugin
{
        DebuggerState m_State;
    public:
        DebuggerGDB();
        ~DebuggerGDB();

        //void SelectionJtag(const wxChoice& mm) const { return (m_ChoiceJtag=mm->GetSelection) ;}
        int Configure();
        int GetConfigurationPriority() const { return 0; }
        int GetConfigurationGroup() const { return cgDebugger; }
        cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
        cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* parent, cbProject* project);
        void BuildMenu(wxMenuBar* menuBar);
        void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);
        bool BuildToolBar(wxToolBar* toolBar);
        void OnAttach(); // fires when the plugin is attached to the application
        void OnRelease(bool appShutDown); // fires when the plugin is released from the application

        void RunCommand(int cmd);
        void Disassemble();
       // void StepByStep();
        void Registers();
        void Backtrace();
        void MemoryDump();
        void RunningThreads();
        int GetRunToCursoridx() const { return m_RunToCursoridx;}

        bool AddBreakpoint(const wxString& file, int line);
        bool AddBreakpoint(const wxString& functionSignature);
        bool RemoveBreakpoint(const wxString& file, int line);
        bool RemoveBreakpoint(const wxString& functionSignature);
        bool RemoveAllBreakpoints(const wxString& file = wxEmptyString);

        void EditorLinesAddedOrRemoved(cbEditor* editor, int startline, int lines);
        void StepByStep();



        void WatchPcInSource();
        //void GetObjSize() {return m_CurrentFrame.objsize};
        int Debug();
        void Continue();
        void Next();
        void NextInstr();
        void Step();
        void StepOut();
        void RunToCursor();
        void ToggleBreakpoint();
        void Break();
        void Stop();
        void MCStop();

        bool Validate(const wxString& line, const char cb);
        bool IsRunning() const { return m_pProcess; }
        int GetExitCode() const { return m_LastExitCode; }

        void SyncEditor(const wxString& filename, int line, bool setMarker = true);

        void Log(const wxString& msg);
        void DebugLog(const wxString& msg);
        void SendCommand(const wxString& cmd);

        DebuggerState& GetState(){ return m_State; }
         unsigned long int GetBreakAddr() const {return m_BreakAddr; }     /// 2012-8-31 lcy
        void BringAppToFront();
        void RefreshConfiguration();

        wxArrayString& GetSearchDirs(cbProject* prj);
        RemoteDebuggingMap& GetRemoteDebuggingMap(cbProject* project = 0);
        wxString GetProjectDir() {return m_pProject->GetBasePath(); }
     //   wxString GetProjectPrvDir() {return m_pProject->GetPrvPath(); }
        wxString  GetRelativePath(const wxString& file);

        void OnProjectLoadingHook(cbProject* project, TiXmlElement* elem, bool loading);

        static void ConvertToGDBFriendly(wxString& str);
        static void ConvertToGDBFile(wxString& str);
        static void ConvertToGDBDirectory(wxString& str, wxString base = _T(""), bool relative = true);
        static void StripQuotes(wxString& str);
    protected:
        void AddSourceDir(const wxString& dir);
    private:
        void DoSwitchToDebuggingLayout();
        void DoSwitchToPreviousLayout();
        void ParseOutput(const wxString& output);
        void ClearActiveMarkFromAllEditors();
        void DoWatches();
        wxString GetEditorWordAtCaret();
        wxString FindDebuggerExecutable(Compiler* compiler);
        int LaunchProcess(const wxString& cmd, const wxString& cwd);
        wxString GetDebuggee(ProjectBuildTarget* target);
        int LaunchJtagProcess(const wxString& cmd);
        bool IsStopped();
        void AddDataBreakpoint();
        bool EnsureBuildUpToDate();
        int DoDebug();
        int SearchProcessByName(const wxString& pname);
        void UDPControlsignal(const wxString& cmd);


        void DoUpdateTargetMenu(int targetIndex);
        int RunNixConsole();
        wxString GetConsoleTty(int ConsolePid);
        void OnMouseClick(wxScintillaEvent& event);
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnDebug(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);
        void OnJtagStall(wxCommandEvent& event);
        void OnSendCommandToGDB(wxCommandEvent& event);
        void OnAddSymbolFile(wxCommandEvent& event);
        void OnBacktrace(wxCommandEvent& event);
        void OnDisassemble(wxCommandEvent& event);
        void OnRegisters(wxCommandEvent& event);
        void OnViewWatches(wxCommandEvent& event);
        void OnBreakpoints(wxCommandEvent& event);
        void OnEditWatches(wxCommandEvent& event);
        void OnContinue(wxCommandEvent& event);
        void OnNext(wxCommandEvent& event);
        void OnNextInstr(wxCommandEvent& event);
        void OnStep(wxCommandEvent& event);
        void OnStepOut(wxCommandEvent& event);
        void OnToggleBreakpoint(wxCommandEvent& event);
        void OnRemoveAllBreakpoints(wxCommandEvent& event);
        void OnAddDataBreakpoint(wxCommandEvent& event);
        void OnRunToCursor(wxCommandEvent& event);
        void OnBreakpointAdd(CodeBlocksEvent& event);
        void OnBreakpointEdit(CodeBlocksEvent& event);
        void OnBreakpointDelete(CodeBlocksEvent& event);
        void OnValueTooltip(CodeBlocksEvent& event);
        void OnEditorOpened(CodeBlocksEvent& event);
        void OnProjectActivated(CodeBlocksEvent& event);
        void OnProjectClosed(CodeBlocksEvent& event);
        void OnCompilerStarted(CodeBlocksEvent& event);
        void OnCompilerFinished(CodeBlocksEvent& event);
        void OnBuildTargetSelected(CodeBlocksEvent& event);
        void OnGDBOutput(wxCommandEvent& event);
        void OnGDBError(wxCommandEvent& event);
        void OnGDBTerminated(wxCommandEvent& event);
        void OnIdle(wxIdleEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnWatchesChanged(wxCommandEvent& event);
		void OnShowFile(wxCommandEvent& event);
        void OnCursorChanged(wxCommandEvent& event);
        void OnAddWatch(wxCommandEvent& event);
        void OnAttachToProcess(wxCommandEvent& event);
        void OnDetach(wxCommandEvent& event);
        void OnSettings(wxCommandEvent& event);
        void OnExamineMemory(wxCommandEvent& event);
        void OnRunningThreads(wxCommandEvent& event);
        void OnSelectTarget(wxCommandEvent& event);

        void OnDebugWindows(wxCommandEvent& event);
        void OnToolInfo(wxCommandEvent& event);


        void OnInfoFrame(wxCommandEvent& event);
        void OnInfoDLL(wxCommandEvent& event);
        void OnInfoFiles(wxCommandEvent& event);
        void OnInfoFPU(wxCommandEvent& event);
        void OnInfoSignals(wxCommandEvent& event);

        void ShowChipType(wxToolBar *toolbar);


        int     m_HookID;
        wxMenu* m_pMenu;
        wxMenu* m_TargetJtag;
        DebugTextCtrlLogger* m_pLog;
        DebugTextCtrlLogger* m_pDbgLog;

        PipedProcess* m_pProcess;
        PipedProcess* m_pJtagProcess;
        wxInputStream* m_pJpStream;
        wxFile JtagLog;
        wxToolBar* m_pTbar;
        int m_RealTargetsStartIndex;
        int m_RealTargetIndex;
        int m_PageIndex;
        int m_DbgPageIndex;
        int m_DbgJtagIndex;
        int m_JtagIndex;
        wxRegEx reSource;
        wxString m_LastCmd;
        wxString m_Variable;
        cbCompilerPlugin* m_pCompiler;
        wxChoice* m_ToolJtagTarget;
        wxChoice* m_ToolChipType;
        bool m_LastExitCode;
        int m_Pid,sim_Pid,jtag_Pid;
        int m_PidToAttach; // for "attach to process"
        int m_RunToCursoridx,m_PrvCursorLine;
        wxString m_LastEval;
        wxRect m_EvalRect;
        wxTimer m_TimerPollDebugger;
        bool m_NoDebugInfo;


        // Set, but was never used.  HC changed to pass to "Start()"
        // Looks like was meant to allow initial step into first instruction of program start
        bool m_BreakOnEntry;

        int m_HaltAtLine;
        bool m_HasDebugLog;
        bool m_HasJtagLog;
        bool m_StoppedOnSignal;

        // current frame info
        StackFrame m_CurrentFrame;

        // extra dialogs
        DebuggerTree* m_pTree;
        DisassemblyDlg* m_pDisassembly;
        CPURegistersDlg* m_pCPURegisters;
        BacktraceDlg* m_pBacktrace;
        BreakpointsDlg* m_pBreakpointsWindow;
        ExamineMemoryDlg* m_pExamineMemoryDlg;
        ThreadsDlg* m_pThreadsDlg;

        cbProject* m_pProject; // keep the currently debugged project handy
        wxString m_ActiveBuildTarget;

        // per-project debugger search-dirs
        typedef std::map<cbProject*, wxArrayString> SearchDirsMap;
        SearchDirsMap m_SearchDirs;

        typedef std::map<cbProject*, RemoteDebuggingMap> ProjectRemoteDebuggingMap;
        ProjectRemoteDebuggingMap m_RemoteDebugging;

        int m_HookId; // project loader hook ID

        // Linux console support
        bool     m_bIsConsole;
        int      m_nConsolePid;
        wxString m_ConsoleTty;
        int      m_JtagOrSim;

        bool m_WaitingCompilerToFinish;
        bool m_Canceled; // flag to avoid re-entering DoDebug when we shouldn't
        unsigned long int m_BreakAddr;
        wxString m_PreviousLayout;
        wxString m_allChipType;

        DECLARE_EVENT_TABLE()
};

#endif // DEBUGGERGDB_H

