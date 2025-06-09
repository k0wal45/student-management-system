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

	//lista student�w
    wxListCtrl* studentsList;

   //lista ocen
    wxListCtrl* gradesList;

	//lista egzamin�w
    wxListCtrl* examsList;

    //Dodane pole do wyszukiwania
    wxTextCtrl* searchCtrl; 

	//Zak�adki
    wxNotebook* notebook;



	//Dodanie oceny (event)
    void OnAddGrade(wxCommandEvent& event);

    //Dodanie oceny
    void ShowAddGradeDialog();

    //Dodanie egzaminu (event)
    void OnAddExam(wxCommandEvent& event);

    //Dodanie egzaminu
    void ShowAddExamDialog();



	//Usuni�cie oceny (event)
	void OnRemoveGrade(wxCommandEvent& event);

    //Usuni�cie oceny
    void ShowRemoveGradeDialog();

	//Usuni�cie egzaminu (event)
    void OnRemoveExam(wxCommandEvent& event);



	//Od�wie�enie listy egzamin�w
    void RefreshExamsList();

    //Od�wie�enie listy ocen
    void RefreshGradesList();


   
    //filtrowanie student�w (event)
    void OnSearch(wxCommandEvent& event);

    //filtrowanie student�w (event)
    void FilterData(const wxString& searchText, bool filterGrades);
};