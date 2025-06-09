#include "MainFrame.h"

//Konstruktor

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
   
    SetBackgroundColour(wxColour(240, 240, 240));

    //Menu
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit this program");
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    //G³ówny sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    //Panel logowania
    ShowLoginPanel();

    //Mo¿liwoœæ zamkniêcia aplikacji
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}


//Wyœwietl panel logowania

void MainFrame::ShowLoginPanel()
{
    mainSizer->Clear(true);
    mainSizer->Add(new LoginPanel(this), 1, wxEXPAND);
    Layout();
}


//Wyœwietl panel admina

void MainFrame::ShowAdminPanel()
{
    mainSizer->Clear(true);
    mainSizer->Add(new AdminPanel(this), 1, wxEXPAND);
    Layout();
}

//Wyœwietl panel nauczyciela

void MainFrame::ShowTeacherPanel(const wxString& teacherName)
{
    mainSizer->Clear(true);
    mainSizer->Add(new TeacherPanel(this, teacherName), 1, wxEXPAND);
    Layout();
}


//Wyœwietl panel studenta

void MainFrame::ShowStudentPanel(const wxString& studentName)
{
    mainSizer->Clear(true);
    mainSizer->Add(new StudentPanel(this, studentName), 1, wxEXPAND);
    Layout();
}



//Zamkniêcie aplikacji

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}


