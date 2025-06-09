#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
class AdminPanel : public wxPanel
{
public:
    //konstruktor
    AdminPanel(wxWindow* parent);

private:
    
    wxNotebook* notebook;

    //Lista studentów
    wxListCtrl* studentsList;

	//Lista nauczycieli
    wxListCtrl* teachersList;

	//dododawanie studentów (event)
    void OnAddStudent(wxCommandEvent& event);

    //dodanie nauczyciela
    void OnAddTeacher(wxCommandEvent& event);

	//Odœwie¿anie listy studentów
    void RefreshTeacherList();

    //Odœwie¿anie listy nauczycieli
    void RefreshStudentList();

    //Dodawanie u¿ytkownika (nauczyciel/student)
    void ShowAddUserDialog(const wxString& role);

	//Usuniêcie nauczyciela
    void OnRemoveTeacher(wxCommandEvent& event);

	//Usuniêcie studenta
    void OnRemoveStudent(wxCommandEvent& event);
};