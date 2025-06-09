//Pliki nag��wkowe
#include "LoginPanel.h"
#include "MainFrame.h"
#include "bcrypt.h"
#include "auth.h"
//Biblioteki
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;


//Konstruktor

LoginPanel::LoginPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
    SetBackgroundColour(wxColour(250, 250, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    //Nag��wek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Student Management System", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 20);

    //Formularz logowania
    wxPanel* formPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
    formPanel->SetBackgroundColour(wxColour(255, 255, 255));

    wxBoxSizer* formSizer = new wxBoxSizer(wxVERTICAL);

	//Pole email i has�o
    wxStaticText* emailLabel = new wxStaticText(formPanel, wxID_ANY, "Email:");
    emailCtrl = new wxTextCtrl(formPanel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1));

    wxStaticText* passwordLabel = new wxStaticText(formPanel, wxID_ANY, "Password:");
    passwordCtrl = new wxTextCtrl(formPanel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1), wxTE_PASSWORD);

    
    //Przyciski
    wxButton* loginButton = new wxButton(formPanel, wxID_ANY, "Login");
    loginButton->Bind(wxEVT_BUTTON, &LoginPanel::OnLogin, this);

 
    //Dodawanie element�w do sizera
    formSizer->Add(emailLabel, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    formSizer->Add(emailCtrl, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(passwordLabel, 0, wxLEFT | wxRIGHT, 10);
    formSizer->Add(passwordCtrl, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(loginButton, 0, wxALL | wxEXPAND, 10);
   
    formPanel->SetSizer(formSizer);
    mainSizer->Add(formPanel, 0, wxALL | wxALIGN_CENTER, 20);

    SetSizer(mainSizer);
}



//Obs�uga logowania

void LoginPanel::OnLogin(wxCommandEvent& event)
{
	//Pobranie warto�ci z p�l
    wxString email = emailCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();
    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());


	//Walidacja p�l
    if (email.empty() || password.empty()) {
        wxMessageBox("Please enter both email and password", "Error", wxOK | wxICON_ERROR);
        return;
    }


    //Wywo�anie funkcji AuthUser
    std::string role = Auth::AuthUser(email.ToStdString(), password.ToStdString());


    //Weryfikacja poprawno�ci danych
    if (role.empty()) {
        wxMessageBox("Invalid email or password.", "Error", wxOK | wxICON_ERROR);
        return;
    }


    //Wy�wietlenie odpowiedniego panelu w zale�no�ci od roli u�ytkownika
    if (role == "admin") {
        frame->ShowAdminPanel();
    }
    else if (role == "teacher") {
        frame->ShowTeacherPanel(email);
    }
    else if (role == "student") {
        frame->ShowStudentPanel(email);
    }
    else {
        wxMessageBox("Unknown user role.", "Error", wxOK | wxICON_ERROR);
    }
    
}



