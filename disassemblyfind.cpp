#include "include/sdk.h"
#include "disassemblyfind.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(disassemblyfind)
	//*)
#endif
//(*InternalHeaders(disassemblyfind)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(disassemblyfind)
//*)

BEGIN_EVENT_TABLE(disassemblyfind,wxDialog)
	//(*EventTable(disassemblyfind)
	//*)
END_EVENT_TABLE()

disassemblyfind::disassemblyfind(wxWindow* parent)
{
	//(*Initialize(disassemblyfind)
	wxXmlResource::Get()->LoadObject(this,parent,_T("disassemblyfind"),_T("wxDialog"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	ComboBox1 = (wxComboBox*)FindWindow(XRCID("ID_COMBOBOX1"));
	StaticLine1 = (wxStaticLine*)FindWindow(XRCID("ID_STATICLINE1"));
	Button1 = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Button2 = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));
	Button3 = (wxButton*)FindWindow(XRCID("ID_BUTTON3"));
	
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&disassemblyfind::OnButton2Click);
	//*)
}

disassemblyfind::~disassemblyfind()
{
	//(*Destroy(disassemblyfind)
	//*)
}


void disassemblyfind::OnButton2Click(wxCommandEvent& event)
{
}
