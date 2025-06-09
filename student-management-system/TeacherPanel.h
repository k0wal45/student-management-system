#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>

class TeacherPanel : public wxPanel
{
public:
	//Konstruktor
    TeacherPanel(wxWindow* parent, const wxString& teacherEmail);
   
private:
	//Nazwa nauczyciela
    wxString teacherEmail;

	//lista studentów
    wxListCtrl* studentsList;

   //lista ocen
    wxListCtrl* gradesList;

	//lista egzaminów
    wxListCtrl* examsList;

    //Dodane pole do wyszukiwania
    wxTextCtrl* searchCtrl; 

	//Zak³adki
    wxNotebook* notebook;



	//Dodanie oceny (event)
    void OnAddGrade(wxCommandEvent& event);

    //Dodanie oceny
    void ShowAddGradeDialog();

    //Dodanie egzaminu (event)
    void OnAddExam(wxCommandEvent& event);

    //Dodanie egzaminu
    void ShowAddExamDialog();



	//Usuniêcie oceny (event)
	void OnRemoveGrade(wxCommandEvent& event);

    //Usuniêcie oceny
    void ShowRemoveGradeDialog();

	//Usuniêcie egzaminu (event)
    void OnRemoveExam(wxCommandEvent& event);



	//Odœwie¿enie listy egzaminów
    void RefreshExamsList();

    //Odœwie¿enie listy ocen
    void RefreshGradesList();


   
    //filtrowanie studentów (event)
    void OnSearch(wxCommandEvent& event);

    //filtrowanie studentów (event)
    void FilterData(const wxString& searchText, bool filterGrades);
};