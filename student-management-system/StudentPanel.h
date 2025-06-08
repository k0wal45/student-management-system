#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
class StudentPanel : public wxPanel
{
public:
    //Stworzenie panelu studenta
    StudentPanel(wxWindow* parent, const wxString& studentName);

private:
    //przechiwanie nazwy studenta
    wxString studentEmail;
    
    wxListCtrl* gradesList;
    wxListCtrl* examsList;
    void ShowStudentDetails();
};