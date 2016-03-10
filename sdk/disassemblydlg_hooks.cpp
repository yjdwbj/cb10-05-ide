/*
 * This file is part of the MC::Tools IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"
#include "disassemblydlg_hooks.h"
#include "wx/wxscintilla.h"

#include <map>

typedef std::map<int, DisassemblyDlgHooks::HookFunctorBase*> HookFunctorsMap;
static HookFunctorsMap s_HookFunctorsMap;
static int s_UniqueID = 0;

int DisassemblyDlgHooks::RegisterHook(DisassemblyDlgHooks::HookFunctorBase* functor)
{
    for (HookFunctorsMap::iterator it = s_HookFunctorsMap.begin(); it != s_HookFunctorsMap.end(); ++it)
    {
        if (it->second == functor)
            return it->first;
    }
    s_HookFunctorsMap[s_UniqueID] = functor;
    return s_UniqueID++;
}

DisassemblyDlgHooks::HookFunctorBase* DisassemblyDlgHooks::UnregisterHook(int id, bool deleteHook)
{
    HookFunctorsMap::iterator it = s_HookFunctorsMap.find(id);
    if (it != s_HookFunctorsMap.end())
    {
        DisassemblyDlgHooks::HookFunctorBase* functor = it->second;
        s_HookFunctorsMap.erase(it);
        if (deleteHook)
        {
            delete functor;
            return 0;
        }
        return functor;
    }
    return 0;
}

bool DisassemblyDlgHooks::HasRegisteredHooks()
{
    return s_HookFunctorsMap.size() != 0;
}

void DisassemblyDlgHooks::CallHooks(DisassemblyDlg* disassemblydlg, wxScintillaEvent& event)
{
    for (HookFunctorsMap::iterator it = s_HookFunctorsMap.begin(); it != s_HookFunctorsMap.end(); ++it)
    {
        DisassemblyDlgHooks::HookFunctorBase* functor = it->second;
        if (functor)
            functor->Call(disassemblydlg, event);
    }
}
