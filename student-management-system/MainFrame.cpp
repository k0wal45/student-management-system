#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    // Ustawienia stylu
    SetBackgroundColour(wxColour(240, 240, 240));

    // Menu
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit this program");
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    // G³ówny sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    // Panel logowania
    ShowLoginPanel();

    // Eventy
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::ShowLoginPanel()
{
    mainSizer->Clear(true);
    mainSizer->Add(new LoginPanel(this), 1, wxEXPAND);
    Layout();
}

void MainFrame::ShowAdminPanel()
{
    mainSizer->Clear(true);
    mainSizer->Add(new AdminPanel(this), 1, wxEXPAND);
    Layout();
}

void MainFrame::ShowTeacherPanel(const wxString& teacherName)
{
    mainSizer->Clear(true);
    mainSizer->Add(new TeacherPanel(this, teacherName), 1, wxEXPAND);
    Layout();
}

void MainFrame::ShowStudentPanel(const wxString& studentName)
{
    mainSizer->Clear(true);
    mainSizer->Add(new StudentPanel(this, studentName), 1, wxEXPAND);
    Layout();
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Student Management System\nVersion 1.0", "About", wxOK | wxICON_INFORMATION);
}