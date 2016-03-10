/*
 * This file is part of the MC::Tools IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifdef __WXMSW__
// this compiler is valid only in windows

#ifndef COMPILERDSP_H
#define COMPILERDSP_H

#include <compiler.h>

class CompilerDSP : public Compiler
{
	public:
		CompilerDSP();
		virtual ~CompilerDSP();
        virtual void Reset();
		virtual void LoadDefaultRegExArray();
        virtual AutoDetectResult AutoDetectInstallationDir();
	protected:
        Compiler * CreateCopy();
	private:
};

#endif // COMPILERDMC_H

#endif // __WXMSW__
