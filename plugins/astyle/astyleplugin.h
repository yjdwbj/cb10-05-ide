/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ASTYLEPLUGIN_H
#define ASTYLEPLUGIN_H

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cbplugin.h> // the base class we 're inheriting
#include <settings.h> // needed to use the MC::Tools SDK

class AStylePlugin : public cbToolPlugin
{
  public:
    AStylePlugin();
    ~AStylePlugin();
    int Configure();
    int GetConfigurationGroup() const { return cgEditor; }
    cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
    void BuildModuleMenu( const ModuleType type, wxMenu* menu, const FileTreeData* data = 0 );
    int Execute();

    void OnFormatActiveFile( wxCommandEvent& event );
    void OnFormatProject( wxCommandEvent& event );
    void FormatFile( const wxString &filename );
    bool FormatEditor( cbEditor *ed );

    void OnAttach(); // fires when the plugin is attached to the application
    void OnRelease(bool appShutDown); // fires when the plugin is released from the application

    DECLARE_EVENT_TABLE()
};

#endif // ASTYLEPLUGIN_H
