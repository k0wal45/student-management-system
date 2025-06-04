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
   //lista ocen
    wxListCtrl* gradesList;
    // Dodane pole do wyszukiwania
    wxTextCtrl* searchCtrl; 
    wxNotebook* notebook;
	//dodanie oceny
    void OnAddGrade(wxCommandEvent& event);
    void ShowAddGradeDialog();

	void OnRemoveGrade(wxCommandEvent& event);
	//filtrowanie studentów
    void OnSearch(wxCommandEvent& event);
    void FilterStudents(const wxString& searchText);


    //aktualizacja listy ocen po dodaniu
    void RefreshGradesList();


    void FilterData(const wxString& searchText, bool filterGrades);
    void RemoveGrade();
    void ShowRemoveGradeDialog();

    //dodanie egzaminu
    void OnAddExam(wxCommandEvent& event);
    void ShowAddExamDialog();
   
};