#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
class AdminPanel : public wxPanel
{
public:
    AdminPanel(wxWindow* parent);

private:
    
    wxNotebook* notebook;
    wxListCtrl* studentsList;
	//dododawanie studentów
    void OnAddStudent(wxCommandEvent& event);
	//dodawanie nauczycieli
    void OnAddTeacher(wxCommandEvent& event);

    void ShowAddUserDialog(const wxString& role);

    void RefreshStudentList();
    void OnRemoveStudent(wxCommandEvent& event);
};