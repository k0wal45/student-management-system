#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
class StudentPanel : public wxPanel
{
public:
    //Konstruktor
    StudentPanel(wxWindow* parent, const wxString& studentName);

private:
    //przechiwanie nazwy studenta
    wxString studentEmail;

	//Lista ocen
    wxListCtrl* gradesList;

	//Lista egzaminów
    wxListCtrl* examsList;

	//Wyœwietl dane studenta
    void ShowStudentDetails();

    //Wyœwietl egzaminy studenta
    void ShowExams();
};