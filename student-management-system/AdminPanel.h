#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>

class AdminPanel : public wxPanel
{
public:
    AdminPanel(wxWindow* parent);

private:
    
    wxNotebook* notebook;
	//dododawanie student�w
    void OnAddStudent(wxCommandEvent& event);
	//dodawanie nauczycieli
    void OnAddTeacher(wxCommandEvent& event);

    void ShowAddUserDialog(const wxString& role);
   
    void OnEditUser(wxCommandEvent& event);

};