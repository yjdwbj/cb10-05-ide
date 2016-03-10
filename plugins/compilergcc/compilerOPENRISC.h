/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef COMPILER_OPENRISC_H
#define COMPILER_OPENRISC_H

#include "compiler.h"

class CompilerOPENRISC : public Compiler
{
    public:
        CompilerOPENRISC();
        virtual ~CompilerOPENRISC();
        virtual void Reset();

        virtual void LoadDefaultRegExArray();
        virtual AutoDetectResult AutoDetectInstallationDir();
    protected:
        virtual Compiler* CreateCopy();
    private:
};

#endif // COMPILER_OPENRISC_H
