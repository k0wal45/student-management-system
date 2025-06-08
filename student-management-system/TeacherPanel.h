#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>

class TeacherPanel : public wxPanel
{
public:
	//stworzenie panelu nauczyciela
    TeacherPanel(wxWindow* parent, const wxString& teacherEmail);
   
private:
	//przechowanie nazwy nauczyciela
    wxString teacherEmail;
	//lista student�w
    wxListCtrl* studentsList;
   //lista ocen
    wxListCtrl* gradesList;
    wxListCtrl* examsList;
    // Dodane pole do wyszukiwania
    wxTextCtrl* searchCtrl; 
    wxNotebook* notebook;
	//dodanie oceny
    void OnAddGrade(wxCommandEvent& event);
    void ShowAddGradeDialog();

	void OnRemoveGrade(wxCommandEvent& event);
	//filtrowanie student�w
    void OnSearch(wxCommandEvent& event);
    void FilterStudents(const wxString& searchText);

    void RefreshExamsList();
    //aktualizacja listy ocen po dodaniu
    void RefreshGradesList();


    void FilterData(const wxString& searchText, bool filterGrades);
    void RemoveGrade();
    void ShowRemoveGradeDialog();

    //dodanie egzaminu
    void OnAddExam(wxCommandEvent& event);
    void ShowAddExamDialog();
   
};