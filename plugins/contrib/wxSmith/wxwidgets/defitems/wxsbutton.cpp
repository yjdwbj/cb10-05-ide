/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 5796 $
* $Id: wxsbutton.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsbutton.cpp $
*/

#include "wxsbutton.h"

namespace
{
    wxsRegisterItem<wxsButton> Reg(_T("Button"),wxsTWidget,_T("Standard"),90);

    WXS_ST_BEGIN(wxsButtonStyles,_T(""))
        WXS_ST_CATEGORY("wxButton")
        WXS_ST(wxBU_LEFT)
        WXS_ST(wxBU_TOP)
        WXS_ST(wxBU_RIGHT)
        WXS_ST(wxBU_BOTTOM)
        WXS_ST(wxBU_EXACTFIT)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxsButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
    WXS_EV_END()
}

wxsButton::wxsButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsButtonEvents,
        wxsButtonStyles),
    Label(_("Label")),
    IsDefault(false)
{}

void wxsButton::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/button.h>"),GetInfo().ClassName,hfInPCH);
            #if wxCHECK_VERSION(2, 9, 0)
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %V, %N);\n"),Label.wx_str());
            #else
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %V, %N);\n"),Label.c_str());
            #endif
            if ( IsDefault ) Codef( _T("%ASetDefault();\n"));
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsButton::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxsButton::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxButton* Preview = new wxButton(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    if ( IsDefault ) Preview->SetDefault();
    return SetupWindow(Preview,Flags);
}

void wxsButton::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxsButton,Label,_("Label"),_T("label"),_T(""),false)
    WXS_BOOL(wxsButton,IsDefault,_("Is default"),_T("default"),false)
}
