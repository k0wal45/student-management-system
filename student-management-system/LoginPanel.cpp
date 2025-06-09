#include "LoginPanel.h"
#include "MainFrame.h"
#include "bcrypt.h"
#include "json.hpp"
#include "fstream"
#include "auth.h"
using json = nlohmann::json;

void LoginPanel::ShowPasswordResetDialog(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Password Reset", wxDefaultPosition, wxSize(400, 200));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Enter your email to reset password:");
    wxTextCtrl* emailCtrl = new wxTextCtrl(panel, wxID_ANY);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
    
    btnSizer->Add(cancelBtn);

    sizer->Add(label, 0, wxALL, 10);
    sizer->Add(emailCtrl, 0, wxALL | wxEXPAND, 10);
    sizer->Add(btnSizer, 0, wxALIGN_CENTER | wxALL, 10); // Poprawione: u¿ywamy btnSizer zamiast sizer

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
        if (emailCtrl->GetValue().empty()) {
            wxMessageBox("Please enter email address", "Error", wxOK | wxICON_ERROR);
        }
        else {
            wxMessageBox("Password reset instructions sent to " + emailCtrl->GetValue(),
                "Success", wxOK | wxICON_INFORMATION);
            dlg.EndModal(wxID_OK);
        }
        }, wxID_OK);

    dlg.ShowModal();
}




LoginPanel::LoginPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
    SetBackgroundColour(wxColour(250, 250, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Nag³ówek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Student Management System",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 20);

    // Formularz logowania
    wxPanel* formPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
    formPanel->SetBackgroundColour(wxColour(255, 255, 255));

    wxBoxSizer* formSizer = new wxBoxSizer(wxVERTICAL);

    // Kontrolki formularza
    wxStaticText* emailLabel = new wxStaticText(formPanel, wxID_ANY, "Email:");
    emailCtrl = new wxTextCtrl(formPanel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1));

    wxStaticText* passwordLabel = new wxStaticText(formPanel, wxID_ANY, "Password:");
    passwordCtrl = new wxTextCtrl(formPanel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1), wxTE_PASSWORD);

    

    // Przyciski
    wxButton* loginButton = new wxButton(formPanel, wxID_ANY, "Login");
    loginButton->Bind(wxEVT_BUTTON, &LoginPanel::OnLogin, this);

 

    // Dodawanie elementów do sizera (TYLKO RAZ!)
    formSizer->Add(emailLabel, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    formSizer->Add(emailCtrl, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(passwordLabel, 0, wxLEFT | wxRIGHT, 10);
    formSizer->Add(passwordCtrl, 0, wxALL | wxEXPAND, 10);
   
    formSizer->Add(loginButton, 0, wxALL | wxEXPAND, 10);
   
    

    formPanel->SetSizer(formSizer);
    mainSizer->Add(formPanel, 0, wxALL | wxALIGN_CENTER, 20);

    SetSizer(mainSizer);
}




void LoginPanel::OnLogin(wxCommandEvent& event)
{
    wxString email = emailCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();
    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
    if (email.empty() || password.empty()) {
        wxMessageBox("Please enter both email and password", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Wywo³anie funkcji AuthUser
    std::string role = Auth::AuthUser(email.ToStdString(), password.ToStdString());

    if (role.empty()) {
        wxMessageBox("Invalid email or password.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Wyœwietlenie odpowiedniego panelu w zale¿noœci od roli u¿ytkownika
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



