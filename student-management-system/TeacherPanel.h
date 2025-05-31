#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>

class TeacherPanel : public wxPanel
{
public:
	//stworzenie panelu nauczyciela
    TeacherPanel(wxWindow* parent, const wxString& teacherName);
   
private:
	//przechowanie nazwy nauczyciela
    wxString teacherName;
	//lista studentów
    wxListCtrl* studentsList;
    // Dodane pole do wyszukiwania
    wxTextCtrl* searchCtrl; 

	//dodanie oceny
    void OnAddGrade(wxCommandEvent& event);
    void ShowAddGradeDialog();

	//filtrowanie studentów
    void OnSearch(wxCommandEvent& event);
    void FilterStudents(const wxString& searchText);
    //dodanie egzaminu
    void OnAddExam(wxCommandEvent& event);
    void ShowAddExamDialog();
   
};