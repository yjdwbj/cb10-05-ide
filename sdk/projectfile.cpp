/*
 * This file is part of the MC::Tools IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6134 $
 * $Id: projectfile.cpp 6134 2010-02-03 17:43:56Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/projectfile.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "projectfile.h"
    #include "projectbuildtarget.h"
    #include "cbproject.h"
    #include "compilerfactory.h"
    #include "manager.h"
    #include "projectmanager.h"
    #include "macrosmanager.h"
    #include "globals.h"
#endif

#include "projectfileoptionsdlg.h"
#include "filefilters.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(FilesList);

ProjectFile::ProjectFile(cbProject* prj) :
    compile(false),
    link(false),
    weight(50),
    editorOpen(false),
    editorPos(0),
    editorTopLine(0),
    editorTabPos(0),
    autoGeneratedBy(0),
    project(prj),
    m_VisualState(fvsNormal)
{
}

ProjectFile::~ProjectFile()
{
    // clear PFDMap
    for (PFDMap::iterator it = m_PFDMap.begin(); it != m_PFDMap.end(); ++it)
    {
        delete it->second;
    }
    m_PFDMap.clear();
}

void ProjectFile::Rename(const wxString& new_name)
{
    wxString path = file.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

    file.Assign(path + new_name);
    relativeFilename = relativeFilename.BeforeLast(wxFILE_SEP_PATH);
    if (!relativeFilename.IsEmpty())
    {
        relativeFilename.Append(wxFILE_SEP_PATH);
    }
    relativeFilename.Append(new_name);

    if (project)
    {
        project->ProjectFileRenamed(this);
        project->CalculateCommonTopLevelPath();
        project->SetModified(true);
    }
    UpdateFileDetails();
}

void ProjectFile::AddBuildTarget(const wxString& targetName)
{
    if (buildTargets.Index(targetName) == wxNOT_FOUND)
        buildTargets.Add(targetName);

    // add this file to the target's list of files
    if (project)
    {
        ProjectBuildTarget* target = project->GetBuildTarget(targetName);
        if (target && !target->m_Files.Find(this))
            target->m_Files.Append(this);
    }

    // also do this for auto-generated files
    for (size_t i = 0; i < generatedFiles.size(); ++i)
        generatedFiles[i]->AddBuildTarget(targetName);
}

void ProjectFile::RenameBuildTarget(const wxString& oldTargetName, const wxString& newTargetName)
{
    int idx = buildTargets.Index(oldTargetName);
    if (idx != wxNOT_FOUND)
        buildTargets[idx] = newTargetName;

    // also do this for auto-generated files
    for (size_t i = 0; i < generatedFiles.size(); ++i)
        generatedFiles[i]->RenameBuildTarget(oldTargetName, newTargetName);
}

void ProjectFile::RemoveBuildTarget(const wxString& targetName)
{
    int idx = buildTargets.Index(targetName);
    if (idx != wxNOT_FOUND)
        buildTargets.RemoveAt(idx);

    // remove this file from the target's list of files
    if (project)
    {
        ProjectBuildTarget* target = project->GetBuildTarget(targetName);
        if (target)
        {
            wxFilesListNode* node = target->m_Files.Find(this);
            if (node)
                target->m_Files.Erase(node);
        }
    }

    // also do this for auto-generated files
    for (size_t i = 0; i < generatedFiles.size(); ++i)
        generatedFiles[i]->RemoveBuildTarget(targetName);
}

bool ProjectFile::ShowOptions(wxWindow* parent)
{
    ProjectFileOptionsDlg dlg(parent, this);
    PlaceWindow(&dlg);
    return dlg.ShowModal() == wxID_OK;
}

wxString ProjectFile::GetBaseName() const
{
    wxFileName fname(relativeFilename);
    fname.SetExt(wxEmptyString);
    return fname.GetFullPath();
}

const wxString& ProjectFile::GetObjName()
{
    if (generatedFiles.size())
    {
        // for files generating other files,
        // report the first generated file's "object name"
        return generatedFiles[0]->GetObjName();
    }

    if (m_ObjName.IsEmpty())
        SetObjName(relativeToCommonTopLevelPath);
    return m_ObjName;
}

void ProjectFile::SetObjName(const wxString& name)
{
    bool extendedObjectNames = project->GetExtendedObjectNamesGeneration();
    wxFileName fname(name);
    m_ObjName = name;
    FileType ft = FileTypeOf(name);
    if (ft == ftResource || ft == ftResourceBin)
    {
        if (extendedObjectNames)
            m_ObjName += FileFilters::RESOURCEBIN_DOT_EXT;
        else
        {
            fname.SetExt(FileFilters::RESOURCEBIN_EXT);
            m_ObjName = fname.GetFullPath();
        }
    }
    else if (ft == ftHeader) // support precompiled headers?
    {
        Compiler* compiler = CompilerFactory::GetCompiler(project->GetCompilerID());
        if (compiler && compiler->GetSwitches().supportsPCH)
        {
            // PCHs are always using the extended name mode (at least for GCC)
            // the extension is set to "h.gch"
            if (project->GetModeForPCH() == pchSourceFile)
                fname.Assign(relativeFilename);
            fname.SetExt(compiler->GetSwitches().PCHExtension);
            m_ObjName = fname.GetFullPath();
        }
    }
    else
    {
        if (project)
        {
            Compiler* compiler = CompilerFactory::GetCompiler(project->GetCompilerID());
            if (compiler)
            {
                if (extendedObjectNames)
                    m_ObjName += _T('.') + compiler->GetSwitches().objectExtension;
                else
                {
                    fname.SetExt(compiler->GetSwitches().objectExtension);
                    m_ObjName = fname.GetFullPath();
                }
            }
        }
        else
        {
            if (extendedObjectNames)
                m_ObjName += _T(".o"); // fallback?
            else
            {
                fname.SetExt(_T(".o"));
                m_ObjName = fname.GetFullPath();
            }
        }
    }
//#ifdef __WXMSW__
//    // special case for windows and files on a different drive
//    if (name.Length() > 1 && name.GetChar(1) == _T(':'))
//    {
//        m_ObjName.Remove(1, 1); // NOTE (mandrav): why remove the colon???
//    }
//#endif
}

// map target to pfDetails
void ProjectFile::UpdateFileDetails(ProjectBuildTarget* target)
{
    if (!project)
        return;

    if (!compile && !link)
        return;

    // update PCH output name (in case project PCH mode was changed)
    if (FileTypeOf(relativeFilename) == ftHeader)
        SetObjName(relativeToCommonTopLevelPath);

    if (!target) // update all targets
    {
        int tcount = project->GetBuildTargetsCount();
        for (int x = 0; x < tcount; ++x)
        {
            ProjectBuildTarget* bt = project->GetBuildTarget(x);
            DoUpdateFileDetails(bt);
        }
    }
    else
        DoUpdateFileDetails(target);
}

void ProjectFile::DoUpdateFileDetails(ProjectBuildTarget* target)
{
    // if we don't belong in this target, abort
    if (!target || buildTargets.Index(target->GetTitle()) == wxNOT_FOUND)
        return;
    // delete old PFD
    pfDetails* pfd = m_PFDMap[target];
    if (pfd)
        pfd->Update(target, this);
    else
    {
        pfd = new pfDetails(target, this);
        m_PFDMap[target] = pfd;
    }
}

const pfDetails& ProjectFile::GetFileDetails(ProjectBuildTarget* target)
{
    pfDetails* pfd = m_PFDMap[target];
    if (!pfd)
    {
        DoUpdateFileDetails(target);
        pfd = m_PFDMap[target];
    }
    return *pfd;
}

FileVisualState ProjectFile::GetFileState() const
{
    return m_VisualState;
}

void ProjectFile::SetFileState(FileVisualState state)
{
    if (state != m_VisualState)
    {
        m_VisualState = state;
        wxTreeCtrl* tree = Manager::Get()->GetProjectManager()->GetTree();
        if (tree && m_TreeItemId.IsOk())
        {
            tree->SetItemImage(m_TreeItemId, (int)state, wxTreeItemIcon_Normal);
            tree->SetItemImage(m_TreeItemId, (int)state, wxTreeItemIcon_Selected);
        }
    }
}

void ProjectFile::SetUseCustomBuildCommand(const wxString& compilerId, bool useCustomBuildCommand)
{
    customBuild[compilerId].useCustomBuildCommand = useCustomBuildCommand;
}

void ProjectFile::SetCustomBuildCommand(const wxString& compilerId, const wxString& newBuildCommand)
{
    customBuild[compilerId].buildCommand = newBuildCommand;
}

bool ProjectFile::GetUseCustomBuildCommand(const wxString& compilerId)
{
    return customBuild[compilerId].useCustomBuildCommand;
}

wxString ProjectFile::GetCustomBuildCommand(const wxString& compilerId)
{
    return customBuild[compilerId].buildCommand;
}

////////////////////////////////////////////////////////////////////////////////
// pfDetails
////////////////////////////////////////////////////////////////////////////////

pfDetails::pfDetails(ProjectBuildTarget* target, ProjectFile* pf)
{
    Update(target, pf);
}

void pfDetails::Update(ProjectBuildTarget* target, ProjectFile* pf)
{
    wxString sep = wxFILE_SEP_PATH;
    wxFileName tmp;

    wxFileName prjbase(target->GetParentProject()->GetBasePath());




    wxString objOut = target ? target->GetObjectOutput() : _T(".");
    wxString depsOut = target ? target->GetDepsOutput() : _T(".");

    // we must replace any macros here early because if the macros expand
    // to absolute paths (like global vars usually do), we 're gonna create
    // invalid filenames below
    Manager::Get()->GetMacrosManager()->ReplaceMacros(objOut, target);
    Manager::Get()->GetMacrosManager()->ReplaceMacros(depsOut, target);
    source_file_native = pf->relativeFilename;
    source_file_absolute_native = pf->file.GetFullPath();

    tmp = pf->GetObjName();
    FileType ft = FileTypeOf(pf->relativeFilename);

    Compiler* compiler = target
                            ? CompilerFactory::GetCompiler(target->GetCompilerID())
                            : CompilerFactory::GetDefaultCompiler();

    // support for precompiled headers
    if (target && ft == ftHeader && compiler && compiler->GetSwitches().supportsPCH)
    {
        switch (target->GetParentProject()->GetModeForPCH())
        {
            case pchSourceDir:
            {
                // if PCH is for a file called all.h, we create
                // all.h.gch/<target>_all.h.gch
                // (that's right: a directory)
                wxString new_gch = target->GetTitle() + _T('_') + pf->GetObjName();
                // make sure we 're not generating subdirs
                size_t len = new_gch.Length();
                for (size_t i = 0; i < len; ++i)
                {
                    wxChar c = new_gch[i];
                    if (c == _T('/') || c == _T('\\') || c == _T('.'))
                        new_gch[i] = _T('_');
                }

                wxFileName fn(source_file_native);
                object_file_native = fn.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) +
                                    fn.GetName() + _T('.') + compiler->GetSwitches().PCHExtension +
                                    wxFILE_SEP_PATH +
                                    new_gch;
                break;
            }

            case pchObjectDir:
            {
                object_file_native = objOut + sep + tmp.GetFullPath();

                break;
            }

            case pchSourceFile:
            {
                object_file_native = pf->GetObjName();
                break;
            }
        }
    }
    else
    {
        if (pf->GetParentProject())
        {
            wxFileName fname(pf->relativeToCommonTopLevelPath);
            if (pf->generatedFiles.size())
            {
                // for files generating other files,
                // use the first generated file's "object name"
                fname.Assign(pf->generatedFiles[0]->relativeToCommonTopLevelPath);
            }
            /* NOTE: In case the source file resides in a different volume
            * than the volume where project file is,
            * then the object file will be created as follows.
            *
            * Project object output dir: C:\Foo\obj\Debug
            * Source: D:\Source\foo.cpp
            * Obj file: C:\Foo\obj\Debug\D\Source\foo.o
            */
            wxString fileVol = fname.GetVolume();
            wxString obj_file_full_path = fname.GetFullPath();
            bool diffVolume = false;

            if (platform::windows
                && (!fileVol.IsEmpty() && !fileVol.IsSameAs(prjbase.GetVolume())))
            {
                objOut += fileVol;
                obj_file_full_path = obj_file_full_path.AfterFirst(_T('\\'));
                diffVolume = true;
            }

            if (ft == ftResource || ft == ftResourceBin)
            {
                if (pf->GetParentProject()->GetExtendedObjectNamesGeneration())
                {
                    object_file_native = objOut + sep + obj_file_full_path;
                    object_file_flat_native = objOut + sep + fname.GetFullName();

                    object_file_native += FileFilters::RESOURCEBIN_DOT_EXT;
                    object_file_flat_native += FileFilters::RESOURCEBIN_DOT_EXT;
                }
                else
                {
                    fname.SetExt(FileFilters::RESOURCEBIN_EXT);
                    wxString obj_file_path = fname.GetFullPath();
                    if (diffVolume)
                        obj_file_path = obj_file_path.AfterFirst(_T('\\'));
                    object_file_native = objOut + sep + obj_file_path;
                    object_file_flat_native = objOut + sep + fname.GetFullName();
                }
            }
            else
            {
                if (pf->GetParentProject()->GetExtendedObjectNamesGeneration())
                {
                    object_file_native = objOut + sep + obj_file_full_path;
                    object_file_flat_native = objOut + sep + fname.GetFullName();

                    if (compiler)
                    {
                        object_file_native += _T('.') + compiler->GetSwitches().objectExtension;
                        object_file_flat_native += _T('.') + compiler->GetSwitches().objectExtension;
                    }
                }
                else
                {
                    if (compiler)
                        fname.SetExt(compiler->GetSwitches().objectExtension);
                    wxString obj_file_path = fname.GetFullPath();
                    if (diffVolume)
                        obj_file_path = obj_file_path.AfterFirst(_T('\\'));
                    object_file_native = objOut + sep + obj_file_path;
                    object_file_flat_native = objOut + sep + fname.GetFullName();
                }
            }
        }
    }
    wxFileName o_file(object_file_native);
    wxFileName o_file_flat(object_file_flat_native);
    o_file.MakeAbsolute(prjbase.GetFullPath());
    o_file_flat.MakeAbsolute(prjbase.GetFullPath());
    object_dir_native = o_file.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    object_file_absolute_native = o_file.GetFullPath();
    object_file_flat_absolute_native = o_file_flat.GetFullPath();
    tmp.SetExt(_T("depend"));
    dep_file_native = depsOut + sep + tmp.GetFullPath();
    wxFileName d_file(dep_file_native);
    d_file.MakeAbsolute(prjbase.GetFullPath());
    dep_dir_native = d_file.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    dep_file_absolute_native = o_file.GetFullPath();

    source_file = UnixFilename(source_file_native);
    QuoteStringIfNeeded(source_file);

    object_file = UnixFilename(object_file_native);
    QuoteStringIfNeeded(object_file);

    object_file_flat = UnixFilename(object_file_flat_native);
    QuoteStringIfNeeded(object_file_flat);

    dep_file = UnixFilename(dep_file_native);
    QuoteStringIfNeeded(dep_file);

    object_dir = UnixFilename(object_dir_native);
    QuoteStringIfNeeded(object_dir);

    dep_dir = UnixFilename(dep_dir_native);
    QuoteStringIfNeeded(dep_dir);

    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_file_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_file_flat_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_dir_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_file_absolute_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_file_flat_absolute_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(dep_file_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(dep_dir_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(dep_file_absolute_native);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(dep_dir);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_dir);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(dep_file);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_file);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(object_file_flat);
    Manager::Get()->GetMacrosManager()->ReplaceEnvVars(source_file);
}
