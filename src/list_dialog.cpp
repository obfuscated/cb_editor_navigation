#include "list_dialog.h"

#include <wx/listctrl.h>
#include <wx/sizer.h>

#include <cbproject.h>

ListDialog::ListDialog(wxWindow *parent, EditorPositions const &positions, unsigned current_position) :
    wxDialog(parent, wxID_ANY, wxT("List navigation positions"),
             wxDefaultPosition, wxSize(600, 400),
             wxDEFAULT_DIALOG_STYLE | wxCLOSE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER)
{
    wxListCtrl *list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);
    bs->Add(list, 1, wxEXPAND | wxALL, 5);
    SetAutoLayout(true);
    SetSizer(bs);

    enum Columns
    {
        ColumnFilename = 0,
        ColumnLine,
        ColumnColumn,
        ColumnProject
    };

    list->InsertColumn(ColumnFilename, wxT("Filename"));
    list->InsertColumn(ColumnLine, wxT("Line"), 50);
    list->InsertColumn(ColumnColumn, wxT("Column"), 50);
    list->InsertColumn(ColumnProject, wxT("Project"), 100);

    unsigned ii = 0;
    for (EditorPositions::const_iterator it = positions.begin(); it != positions.end(); ++it, ++ii)
    {
        wxListItem item;
        item.SetId(list->GetItemCount());
        item.SetColumn(ColumnFilename);
        item.SetText(it->GetFilename());
        list->InsertItem(item);

        item.SetColumn(ColumnLine);
        item.SetText(wxString::Format(wxT("%d"), it->GetLine() + 1));
        list->SetItem(item);

        item.SetColumn(ColumnColumn);
        item.SetText(wxString::Format(wxT("%d"), it->GetPosInLine() + 1));
        list->SetItem(item);

        item.SetColumn(ColumnProject);
        if (it->GetProject())
            item.SetText(it->GetProject()->GetTitle());
        else
            item.SetText(wxEmptyString);
        list->SetItem(item);

        if (ii == current_position)
        {
            list->SetItemTextColour(item.GetId(), *wxWHITE);
            list->SetItemBackgroundColour(item.GetId(), *wxRED);
        }
    }

    list->SetColumnWidth(ColumnFilename, wxLIST_AUTOSIZE);
    list->SetColumnWidth(ColumnProject, wxLIST_AUTOSIZE);
}
