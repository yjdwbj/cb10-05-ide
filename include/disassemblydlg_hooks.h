/*
 * This file is part of the MC::Tools IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef DISASSEMBLYDLG_HOOKS_H
#define DISASSEMBLYDLG_HOOKS_H

#include "settings.h"
class wxScintilla;
class DisassemblyDlg;
class wxScintillaEvent;
class DebuggerGDB;

/** Provides static functions to add hooks to the project loading/saving procedure. */
namespace DisassemblyDlgHooks
{
    /** Abstract base hook functor interface. */
    class DLLIMPORT HookFunctorBase
    {
        public:
            virtual ~HookFunctorBase(){}
            virtual void Call(DisassemblyDlg*, wxScintillaEvent&) const = 0;
    };

    /** Functor class for use as a project loading/saving hook.
      * Passed as the first parameter in RegisterHook() and
      * UnregisterHook().
      *
      * example:
      * DisassemblyDlgHooks::HookFunctorBase* myhook = new DisassemblyDlgHooks::HookFunctor<MyClass>(this, &MyClass::OnHookCalled);
      * int id = DisassemblyDlgHooks::RegisterHook(myhook);
      * ...
      * (and before your class' destruction - or earlier):
      * DisassemblyDlgHooks::UnregisterHook(id, true);
      *
      * Member functions used as hook callbacks must have the following signature:
      * void YourFunctionName(cbProject*, TiXmlElement*, bool)
      *
      * Use normal TinyXML procedures to work with the TiXmlElement* argument.
      * The isLoading argument is true if your hook is called when the project is being loaded,
      * and false when the project is saved.
      */
    template<class T> class HookFunctor : public HookFunctorBase
    {
        public:
            typedef void (T::*Func)(DisassemblyDlg*, wxScintillaEvent&);
            HookFunctor(T* obj, Func func)
                : m_pObj(obj),
                m_pFunc(func)
            {}
            virtual void Call(DisassemblyDlg* editor, wxScintillaEvent& event) const
            {
                if (m_pObj && m_pFunc)
                    (m_pObj->*m_pFunc)(editor, event);
            }
        protected:
            T* m_pObj;
            Func m_pFunc;
    };

    /** Register a project loading/saving hook.
      * @param functor The functor to use as a callback.
      * @return An ID. Use this to unregister your hook later.
      */
    extern DLLIMPORT int RegisterHook(HookFunctorBase* functor);
    /** Unregister a previously registered project loading/saving hook.
      * @param id The hook's ID. You should have the ID from when RegisterHook() was called.
      * @param deleteHook If true, the hook will be deleted (default). If not, it's
      * up to you to delete it.
      * @return The functor. If @c deleteHook was true, it always returns NULL.
      */
    extern DLLIMPORT HookFunctorBase* UnregisterHook(int id, bool deleteHook = true);
    /** Are there any hooks registered?
      * @return True if any hooks are registered, false if none.
      */
    extern DLLIMPORT bool HasRegisteredHooks();
    /** Call all registered hooks using the supplied parameters.
      * This is called by ProjectLoader.
      * @param editor The editor in question.
      * @param event Paremeter (wxScintilla event) to provide to the registered hook
      */
    extern DLLIMPORT void CallHooks(DisassemblyDlg* editor, wxScintillaEvent& event);
};

#endif // DISASSEMBLYDLG_HOOKS_H
