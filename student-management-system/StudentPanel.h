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

	//Lista egzamin�w
    wxListCtrl* examsList;

	//Wy�wietl dane studenta
    void ShowStudentDetails();

    //Wy�wietl egzaminy studenta
    void ShowExams();
};