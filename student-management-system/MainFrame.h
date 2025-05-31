#pragma once
#include <wx/wx.h>
#include "LoginPanel.h"
#include "AdminPanel.h"
#include "TeacherPanel.h"
#include "StudentPanel.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
	//wy�wietl panel logowania
	void ShowLoginPanel(); 
	//wy�wietl panel admina
    void ShowAdminPanel(); 
	//wy�wietl panel nauczyciela
	void ShowTeacherPanel(const wxString& teacherName); 
	//wy�wietl panel studenta
	void ShowStudentPanel(const wxString& studentName); 

private:
	// g��wny sizer do zarz�dzania uk�adem okna
	wxBoxSizer* mainSizer;
	AdminPanel* adminPanel;  // Dodaj t� lini�
	TeacherPanel* teacherPanel;
	StudentPanel* studentPanel;
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	
	
};
