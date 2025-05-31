#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>

class StudentPanel : public wxPanel
{
public:
    //Stworzenie panelu studenta
    StudentPanel(wxWindow* parent, const wxString& studentName);

private:
	//przechiwanie nazwy studenta
    wxString studentName;
};