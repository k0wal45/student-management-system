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
	//wyœwietl panel logowania
	void ShowLoginPanel(); 
	//wyœwietl panel admina
    void ShowAdminPanel(); 
	//wyœwietl panel nauczyciela
	void ShowTeacherPanel(const wxString& teacherName); 
	//wyœwietl panel studenta
	void ShowStudentPanel(const wxString& studentName); 

private:
	// g³ówny sizer do zarz¹dzania uk³adem okna
	wxBoxSizer* mainSizer;
	AdminPanel* adminPanel;  // Dodaj tê liniê
	TeacherPanel* teacherPanel;
	StudentPanel* studentPanel;
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	
	
};
