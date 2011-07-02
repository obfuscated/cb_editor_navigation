#ifndef _EDITOR_NAVIGATION_LIST_DIALOG_H_
#define _EDITOR_NAVIGATION_LIST_DIALOG_H_

#include <wx/dialog.h>

#include "common.h"

class ListDialog : public wxDialog
{
public:
    ListDialog(wxWindow *parent, EditorPositions const &positions, unsigned current_position);
};

#endif // _EDITOR_NAVIGATION_LIST_DIALOG_H_
