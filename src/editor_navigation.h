/***************************************************************
 * Name:      editor_navigation
 * Purpose:   Code::Blocks plugin
 * Author:    Teodor Petrov a.k.a obfuscated (fuscated@gmail.com)
 * Created:   2010-09-07
 * Copyright: Teodor Petrov a.k.a obfuscated
 * License:   GPL
 **************************************************************/

#ifndef EDITOR_NAVIGATION_H_INCLUDED
#define EDITOR_NAVIGATION_H_INCLUDED


// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cbplugin.h> // for "class cbPlugin"

#include "common.h"

class Editor_Navigation : public cbPlugin
{
    public:
        Editor_Navigation();
        virtual ~Editor_Navigation();
        virtual int Configure();
        virtual int GetConfigurationPriority() const { return 50; }
        virtual int GetConfigurationGroup() const { return cgUnknown; }
        virtual cbConfigurationPanel* GetConfigurationPanel(wxWindow* /*parent*/){ return 0; }
        virtual cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* /*parent*/, cbProject* /*project*/){ return 0; }
        virtual void BuildMenu(wxMenuBar* menuBar);
        virtual void BuildModuleMenu(const ModuleType /*type*/, wxMenu* /*menu*/, const FileTreeData* /*data = 0*/) {}
        virtual bool BuildToolBar(wxToolBar* toolBar);
    protected:
        virtual void OnAttach();
        virtual void OnRelease(bool appShutDown);

    private:
        void OnEditorActivated(CodeBlocksEvent &event);
        void OnEditorUpdateUI(CodeBlocksEvent &event);
        void OnProjectClose(CodeBlocksEvent &event);
        void OnTimer(wxTimerEvent &event);
        void OnListDialog(wxCommandEvent &event);
        void OnClear(wxCommandEvent &event);
        void OnForward(wxCommandEvent &event);
        void OnBackward(wxCommandEvent &event);

        void OnForwardUpdateUI(wxUpdateUIEvent &event);
        void OnBackwardUpdateUI(wxUpdateUIEvent &event);
    private:

        void SwitchTo(EditorPosition const &pos);
    private:
        wxTimer m_timer;

        EditorPositions m_positions;
        EditorPosition m_last_position;
        unsigned m_current_index;
    private:
        DECLARE_EVENT_TABLE();
};

#endif // EDITOR_NAVIGATION_H_INCLUDED
