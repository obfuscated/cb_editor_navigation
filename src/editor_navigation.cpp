
#include "editor_navigation.h"
#include <wx/xrc/xmlres.h>
#include <sdk.h> // Code::Blocks SDK
#include <cbeditor.h>
#include <cbstyledtextctrl.h>
#include <configurationpanel.h>
#include <editormanager.h>
#include <logmanager.h>
#include <projectfile.h>
#include <projectmanager.h>

#include "list_dialog.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<Editor_Navigation> reg(_T("Editor_Navigation"));

    int const c_id_timer = wxNewId();
    int const c_id_list_dialog = wxNewId();
    int const c_id_clear = wxNewId();
    int const c_id_forward= XRCID("idEditorNavigationForward");
    int const c_id_backward = XRCID("idEditorNavigationBackward");
}


// events handling
BEGIN_EVENT_TABLE(Editor_Navigation, cbPlugin)
    EVT_TIMER(c_id_timer, Editor_Navigation::OnTimer)
    EVT_MENU(c_id_list_dialog, Editor_Navigation::OnListDialog)
    EVT_MENU(c_id_clear, Editor_Navigation::OnClear)
    EVT_MENU(c_id_forward, Editor_Navigation::OnForward)
    EVT_MENU(c_id_backward, Editor_Navigation::OnBackward)
    EVT_UPDATE_UI(c_id_forward, Editor_Navigation::OnForwardUpdateUI)
    EVT_UPDATE_UI(c_id_backward, Editor_Navigation::OnBackwardUpdateUI)
END_EVENT_TABLE()

// constructor
Editor_Navigation::Editor_Navigation()
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("Editor_Navigation.zip")))
    {
        NotifyMissingFile(_T("Editor_Navigation.zip"));
    }
}

// destructor
Editor_Navigation::~Editor_Navigation()
{
}

void Editor_Navigation::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...

    typedef cbEventFunctor<Editor_Navigation, CodeBlocksEvent> cbEventFunctor;


    Manager* manager = Manager::Get();
    manager->RegisterEventSink(cbEVT_EDITOR_ACTIVATED, new cbEventFunctor(this, &Editor_Navigation::OnEditorActivated));
    manager->RegisterEventSink(cbEVT_EDITOR_UPDATE_UI, new cbEventFunctor(this, &Editor_Navigation::OnEditorUpdateUI));
    manager->RegisterEventSink(cbEVT_PROJECT_CLOSE, new cbEventFunctor(this, &Editor_Navigation::OnProjectClose));

    m_timer.SetOwner(this, c_id_timer);
    m_current_index = 0;
}

void Editor_Navigation::OnRelease(bool /*appShutDown*/)
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be FALSE...

//    Manager* manager = Manager::Get();
//    manager->RemoveAllEventSinksFor(this);
}

int Editor_Navigation::Configure()
{
    //create and display the configuration dialog for your plugin
    cbConfigurationDialog dlg(Manager::Get()->GetAppWindow(), wxID_ANY, _("Your dialog title"));
    cbConfigurationPanel* panel = GetConfigurationPanel(&dlg);
    if (panel)
    {
        dlg.AttachConfigurationPanel(panel);
        PlaceWindow(&dlg);
        return dlg.ShowModal() == wxID_OK ? 0 : -1;
    }
    return -1;
}

void Editor_Navigation::BuildMenu(wxMenuBar* menuBar)
{
    int index = menuBar->FindMenu(_("&View"));
    if (index != wxNOT_FOUND)
    {
        wxMenu *submenu = new wxMenu;

        wxMenu *menu = menuBar->GetMenu(index);
        menu->AppendSubMenu(submenu, _("&Navigation"), wxEmptyString);
        submenu->Append(c_id_forward, _("&Forward"), _("Move the cursor to the next item in the browse history"));
        submenu->Append(c_id_backward, _("&Backward"), _("Move the cursor to the previous item in the browse history"));
        submenu->AppendSeparator();
        submenu->Append(c_id_clear, _("&Clear"), _("Clears the navigation history"));
        submenu->AppendSeparator();
        submenu->Append(c_id_list_dialog, _("&List"), _("Show a list with history of cursor positions"));
    }
}

bool Editor_Navigation::BuildToolBar(wxToolBar* toolBar)
{
    wxString toolbar_resource = wxT("editor_navigation_toolbar_");
    const int toolbarSize = Manager::Get()->GetToolbarImageSize();
    if (toolbarSize == 16)
        toolbar_resource += wxT("16x16");
    else
        toolbar_resource += wxT("22x22");
    Manager::AddonToolBar(toolBar, toolbar_resource);
    toolBar->Realize();
    toolBar->SetInitialSize();
    return true;
}

void Editor_Navigation::OnEditorActivated(CodeBlocksEvent &event)
{
    OnEditorUpdateUI(event);
}

void Editor_Navigation::OnProjectClose(CodeBlocksEvent &event)
{
    cbProject *project = event.GetProject();
    if (!project)
        return;

    EditorPositions temp_positions;
    unsigned new_index = 0;
    unsigned ii = 0;

    for (EditorPositions::iterator it = m_positions.begin(); it != m_positions.end(); ++it, ++ii)
    {
        if (it->GetProject() != project)
        {
            temp_positions.push_back(*it);
            if (ii < m_current_index)
                ++new_index;
        }
    }

    std::swap(m_positions, temp_positions);
    m_current_index = new_index;

    size_t project_count = Manager::Get()->GetProjectManager()->GetProjects()->GetCount();
    if (project_count == 0)
        m_positions.clear();

    if (!m_positions.empty())
    {
        m_current_index = std::min(m_current_index, static_cast<unsigned>(m_positions.size() - 1));
        SwitchTo(m_positions[m_current_index]);
    }
    else
        m_current_index = 0;
}

void Editor_Navigation::OnEditorUpdateUI(CodeBlocksEvent &event)
{
//    LogManager &log = *Manager::Get()->GetLogManager();
//    log.Log(wxT("Editor_Navigation::OnEditorUpdateUI"));
    EditorBase *ed = event.GetEditor();
    if (ed && ed->IsBuiltinEditor())
    {
        cbEditor *editor = static_cast<cbEditor*>(ed);
        cbStyledTextCtrl *control = editor->GetControl();
        if (control)
        {
            int pos = control->GetCurrentPos();
            int line = control->LineFromPosition(pos);
            int pos_in_line = control->GetColumn(pos);// pos - control->GetLineEndPosition(line + 1);

//            log.Log(wxString::Format(wxT("--> %s; [%d:%d:%d]\n"),
//                                     editor->GetFilename().c_str(), pos, line, pos_in_line));

            cbProject *project;
            project = (editor->GetProjectFile()) ? editor->GetProjectFile()->GetParentProject() : NULL;
            m_last_position = EditorPosition(project, editor->GetFilename(), line, pos_in_line, pos);
            m_timer.Start(100, wxTIMER_ONE_SHOT);
        }
    }
}

void Editor_Navigation::OnTimer(wxTimerEvent & /*event*/)
{
//    LogManager &log = *Manager::Get()->GetLogManager();
//    log.Log(wxT("Editor_Navigation::OnTimer"));

    if (!m_positions.empty())
    {
        if (m_positions.back() == m_last_position)
            return;
        if (m_current_index != (m_positions.size() - 1) && m_last_position == m_positions[m_current_index])
            return;
    }

    if (m_positions.size() > 1 && m_current_index < m_positions.size() - 1)
    {
        EditorPositions::iterator start = m_positions.begin();
        std::advance(start, m_current_index + 1);
        m_positions.erase(start, m_positions.end());
    }
    if (!m_positions.empty() && m_last_position.IsOnTheSameLine(m_positions.back()))
        m_positions.back() = m_last_position;
    else
        m_positions.push_back(m_last_position);
    m_current_index = m_positions.size() - 1;
}

void Editor_Navigation::OnListDialog(wxCommandEvent & /*event*/)
{
    ListDialog dialog(Manager::Get()->GetAppWindow(), m_positions, m_current_index);

    dialog.ShowModal();
}

void Editor_Navigation::OnClear(wxCommandEvent & /*event*/)
{
    EditorPosition pos;
    if (!m_positions.empty())
        pos = m_positions.back();
    m_positions.clear();
    m_positions.push_back(pos);
    m_current_index = 0;
}

void Editor_Navigation::SwitchTo(EditorPosition const &pos)
{
//    LogManager &log = *Manager::Get()->GetLogManager();
//    log.Log(wxString::Format(wxT("Editor_Navigation::SwitchTo(%s, %d, %d, %d)"),
//                             pos.GetFilename().c_str(), pos.GetLine(), pos.GetPosInLine(), pos.GetPos())
//            );

    cbEditor *editor = Manager::Get()->GetEditorManager()->Open(pos.GetFilename());
    if (editor && editor->GetControl())
    {
//        log.Log(wxT("----- found editor, trying to switch!------ "));
        cbStyledTextCtrl *control = editor->GetControl();
        editor->GotoLine(control->LineFromPosition(pos.GetPos()), true);
        control->GotoPos(pos.GetPos());
    }
}

void Editor_Navigation::OnForward(wxCommandEvent & /*event*/)
{
//    LogManager &log = *Manager::Get()->GetLogManager();
//    log.Log(wxT("Editor_Navigation::OnForward"));

    if (m_current_index == m_positions.size() - 1)
        return;
    m_current_index++;
    SwitchTo(m_positions[m_current_index]);
}

void Editor_Navigation::OnBackward(wxCommandEvent & /*event*/)
{
//    LogManager &log = *Manager::Get()->GetLogManager();
//    log.Log(wxT("Editor_Navigation::OnBackward"));

    if (m_current_index < 1)
        return;
    m_current_index--;
    SwitchTo(m_positions[m_current_index]);
}

void Editor_Navigation::OnForwardUpdateUI(wxUpdateUIEvent &event)
{
    if (m_positions.size() > 1 && m_current_index < m_positions.size() - 1)
        event.Enable(true);
    else
        event.Enable(false);
}

void Editor_Navigation::OnBackwardUpdateUI(wxUpdateUIEvent &event)
{
    if (m_positions.size() > 1 && m_current_index > 0)
        event.Enable(true);
    else
        event.Enable(false);
}
