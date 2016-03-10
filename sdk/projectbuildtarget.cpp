/*
 * This file is part of the MC::Tools IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 4909 $
 * $Id: projectbuildtarget.cpp 4909 2008-02-27 13:15:26Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/projectbuildtarget.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "projectbuildtarget.h" // class's header file
    #include "cbproject.h"
    #include "manager.h"
    #include "projectmanager.h"
    #include "macrosmanager.h"
    #include "globals.h"
#endif

void ProjectBuildTarget::MirrorCustomCurSettings()
{
    // run just once
    if (m_Mirrored)
        return;

    // keep the current settings safe
    // so we can compare them when saving: this way we can only save what's
    // different from the defaults

    m_CMirror.AutoCheckFilesChange = m_AutoDetectFileChange;
    m_CMirror.GeneraterEactLit = m_EachLit;
    m_CMirror.UseFlatsObjects = m_UseFlatObjects;
    m_CMirror.UseFullSourcePaths = m_UseFullSourcePaths;
    m_Mirrored = true;
}

bool ProjectBuildTarget::CompareMirror()
{
    if( (m_CMirror.AutoCheckFilesChange != m_AutoDetectFileChange)||
       (m_CMirror.GeneraterEactLit!= m_EachLit )||
       (m_CMirror.UseFlatsObjects != m_UseFlatObjects)||
      (m_CMirror.UseFullSourcePaths != m_UseFullSourcePaths))
      SetModified(true);
}


// class constructor
ProjectBuildTarget::ProjectBuildTarget(cbProject* parentProject) : m_Project(parentProject),m_EachLit(false)
{
    m_BuildWithAll = false;
    m_CreateStaticLib = true;
    m_CreateDefFile = true;
    m_UseConsoleRunner = true;
    m_EachLit = false;
    m_AutoDetectFileChange = false;
    m_UseFlatObjects = false;
    m_UseFullSourcePaths = false;
}

// class destructor
ProjectBuildTarget::~ProjectBuildTarget()
{
}

cbProject* ProjectBuildTarget::GetParentProject()
{
    return m_Project;
}

wxString ProjectBuildTarget::GetFullTitle() const
{
    return m_Project->GetTitle() + _T(" - ") + GetTitle();
}

const wxString & ProjectBuildTarget::GetExternalDeps() const
{
    return m_ExternalDeps;
}

void ProjectBuildTarget::SetExternalDeps(const wxString& deps)
{
    if (m_ExternalDeps != deps)
    {
        m_ExternalDeps = deps;
        SetModified(true);
    }
}

const wxString & ProjectBuildTarget::GetAdditionalOutputFiles() const
{
    return m_AdditionalOutputFiles;
}

void ProjectBuildTarget::SetAdditionalOutputFiles(const wxString& files)
{
    if (m_AdditionalOutputFiles != files)
    {
        m_AdditionalOutputFiles = files;
        SetModified(true);
    }
}

bool ProjectBuildTarget::GetIncludeInTargetAll() const
{
	return m_BuildWithAll;
}

void ProjectBuildTarget::SetIncludeInTargetAll(bool buildIt)
{
	if (m_BuildWithAll != buildIt)
	{
        m_BuildWithAll = buildIt;
        SetModified(true);
	}
}

bool ProjectBuildTarget::GetCreateDefFile() const
{
    return m_CreateDefFile;
}

void ProjectBuildTarget::SetCreateDefFile(bool createIt)
{
    if (m_CreateDefFile != createIt)
    {
        m_CreateDefFile = createIt;
        SetModified(true);
    }
}

bool ProjectBuildTarget::GetCreateStaticLib()
{
    return m_CreateStaticLib;
}

void ProjectBuildTarget::SetCreateStaticLib(bool createIt)
{
    if (m_CreateStaticLib != createIt)
    {
        m_CreateStaticLib = createIt;
        SetModified(true);
    }
}

bool ProjectBuildTarget::GetUseConsoleRunner() const
{
    return GetTargetType() == ttConsoleOnly ? m_UseConsoleRunner : false;
}

void ProjectBuildTarget::SetUseConsoleRunner(bool useIt)
{
    if (GetTargetType() == ttConsoleOnly && useIt != m_UseConsoleRunner)
    {
        m_UseConsoleRunner = useIt;
        SetModified(true);
    }
}

void ProjectBuildTarget::SetTargetType(const TargetType& pt)
{
	TargetType ttold = GetTargetType();
	CompileTargetBase::SetTargetType(pt);
	if (ttold != GetTargetType() && GetTargetType() == ttConsoleOnly)
        SetUseConsoleRunner(true); // by default, use console runner
}

// target dependencies: targets to be compiled (if necessary) before this one
void ProjectBuildTarget::AddTargetDep(ProjectBuildTarget* target) {
	m_TargetDeps.Add(target);
}

// get the list of dependency targets of this target
BuildTargets& ProjectBuildTarget::GetTargetDeps() {
	return m_TargetDeps;
}

