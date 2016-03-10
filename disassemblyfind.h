#ifndef DISASSEMBLYFIND_H
#define DISASSEMBLYFIND_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(disassemblyfind)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/combobox.h>
	//*)
#endif
//(*Headers(disassemblyfind)
#include <wx/statline.h>
//*)

class disassemblyfind: public wxDialog
{
	public:

		disassemblyfind(wxWindow* parent);
		virtual ~disassemblyfind();

		//(*Declarations(disassemblyfind)
		wxButton* Button1;
		wxStaticText* StaticText1;
		wxButton* Button2;
		wxButton* Button3;
		wxStaticLine* StaticLine1;
		wxComboBox* ComboBox1;
		//*)

	protected:

		//(*Identifiers(disassemblyfind)
		//*)

	private:

		//(*Handlers(disassemblyfind)
		void OnButton2Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
