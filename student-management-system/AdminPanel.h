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

    //Lista student�w
    wxListCtrl* studentsList;

	//Lista nauczycieli
    wxListCtrl* teachersList;

	//dododawanie student�w (event)
    void OnAddStudent(wxCommandEvent& event);

    //dodanie nauczyciela
    void OnAddTeacher(wxCommandEvent& event);

	//Od�wie�anie listy student�w
    void RefreshTeacherList();

    //Od�wie�anie listy nauczycieli
    void RefreshStudentList();

    //Dodawanie u�ytkownika (nauczyciel/student)
    void ShowAddUserDialog(const wxString& role);

	//Usuni�cie nauczyciela
    void OnRemoveTeacher(wxCommandEvent& event);

	//Usuni�cie studenta
    void OnRemoveStudent(wxCommandEvent& event);
};