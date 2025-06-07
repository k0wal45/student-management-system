#include "LoginPanel.h"
#include "MainFrame.h"
void LoginPanel::ShowPasswordResetDialog(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Password Reset", wxDefaultPosition, wxSize(400, 200));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Enter your email to reset password:");
    wxTextCtrl* emailCtrl = new wxTextCtrl(panel, wxID_ANY);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* resetBtn = new wxButton(panel, wxID_OK, "Reset");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
    btnSizer->Add(resetBtn, 0, wxRIGHT, 10);
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

    wxStaticText* roleLabel = new wxStaticText(formPanel, wxID_ANY, "Role:");
    roleChoice = new wxChoice(formPanel, wxID_ANY, wxDefaultPosition, wxSize(250, -1));
    roleChoice->Append("Student");
    roleChoice->Append("Teacher");
    roleChoice->Append("Admin");
    roleChoice->SetSelection(0);

    // Przyciski
    wxButton* loginButton = new wxButton(formPanel, wxID_ANY, "Login");
    loginButton->Bind(wxEVT_BUTTON, &LoginPanel::OnLogin, this);

    wxButton* registerButton = new wxButton(formPanel, wxID_ANY, "Register");
    registerButton->Bind(wxEVT_BUTTON, &LoginPanel::OnRegister, this);

    wxButton* resetPasswordBtn = new wxButton(formPanel, wxID_ANY, "Reset Password");
    resetPasswordBtn->Bind(wxEVT_BUTTON, &LoginPanel::ShowPasswordResetDialog, this);

    // Dodawanie elementów do sizera (TYLKO RAZ!)
    formSizer->Add(emailLabel, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    formSizer->Add(emailCtrl, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(passwordLabel, 0, wxLEFT | wxRIGHT, 10);
    formSizer->Add(passwordCtrl, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(roleLabel, 0, wxLEFT | wxRIGHT, 10);
    formSizer->Add(roleChoice, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(loginButton, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(registerButton, 0, wxALL | wxEXPAND, 10);
    formSizer->Add(resetPasswordBtn, 0, wxALL | wxEXPAND, 10);

    formPanel->SetSizer(formSizer);
    mainSizer->Add(formPanel, 0, wxALL | wxALIGN_CENTER, 20);

    SetSizer(mainSizer);
}




void LoginPanel::OnLogin(wxCommandEvent& event)
{
    wxString email = emailCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();
    wxString role = roleChoice->GetStringSelection();

    if (email.empty() || password.empty()) {
        wxMessageBox("Please enter both email and password", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Symulacja logowania
    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
    if (role == "Admin") {
        frame->ShowAdminPanel();
    }
    else if (role == "Teacher") {
        frame->ShowTeacherPanel(email);
    }
    else {
        frame->ShowStudentPanel(email);
    }
}

void LoginPanel::OnRegister(wxCommandEvent& event)
{
    ShowRegistrationDialog();
}

// Wyœwietlenie okna rejestracji
void LoginPanel::ShowRegistrationDialog()
{
    // Pobierz rozmiar g³ównego okna
    wxSize parentSize = GetParent()->GetSize();
     wxSize dialogSize(parentSize.GetWidth() * 0.8, parentSize.GetHeight() * 0.8);
    wxDialog dlg(this, wxID_ANY, "Register New Account", wxDefaultPosition, wxSize(500,600),
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);


    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Create New Account");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    
    //sizer dla tytu³u
    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 20);

    // Sizer dla formularza - teraz pionowy uk³ad
    wxBoxSizer* formSizer = new wxBoxSizer(wxVERTICAL);

    // Funkcja pomocnicza do tworzenia par etykieta-input
    auto AddLabelAndInput = [&](wxBoxSizer* sizer, const wxString& label, wxWindow* inputCtrl) {
        wxStaticText* labelCtrl = new wxStaticText(panel, wxID_ANY, label);
        labelCtrl->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        wxBoxSizer* itemSizer = new wxBoxSizer(wxVERTICAL);
        itemSizer->Add(labelCtrl, 0, wxLEFT | wxTOP, 5);
        itemSizer->Add(inputCtrl, 0, wxALL | wxEXPAND, 5);

        sizer->Add(itemSizer, 0, wxALL | wxEXPAND, 5);
        };

    // Kontrolki formularza
    wxTextCtrl* firstNameCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25));
    wxTextCtrl* lastNameCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25));
    wxTextCtrl* emailCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25));
    wxTextCtrl* passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxTE_PASSWORD);
    wxChoice* roleChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 25));
    roleChoice->Append("Student");
    roleChoice->Append("Teacher");
    roleChoice->SetSelection(0);

    // Dodajemy pary etykieta-input do formularza
    AddLabelAndInput(formSizer, "First Name:", firstNameCtrl);
    AddLabelAndInput(formSizer, "Last Name:", lastNameCtrl);
    AddLabelAndInput(formSizer, "Email:", emailCtrl);
    AddLabelAndInput(formSizer, "Password:", passwordCtrl);

    // Specjalna obs³uga dla Choice (combobox)
    wxStaticText* roleLabel = new wxStaticText(panel, wxID_ANY, "Role:");
    roleLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxBoxSizer* roleSizer = new wxBoxSizer(wxVERTICAL);
    roleSizer->Add(roleLabel, 0, wxLEFT | wxTOP, 5);
    roleSizer->Add(roleChoice, 0, wxALL | wxEXPAND, 5);
    formSizer->Add(roleSizer, 0, wxALL | wxEXPAND, 5);

    // Przyciski
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* registerButton = new wxButton(panel, wxID_OK, "Register");
    registerButton->SetMinSize(wxSize(120, 35));
    wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel");
    cancelButton->SetMinSize(wxSize(120, 35));
    buttonSizer->Add(registerButton, 0, wxALL, 10);
    buttonSizer->Add(cancelButton, 0, wxALL, 10);

    // Dodaj wszystko do g³ównego sizera
    sizer->Add(formSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 30);
   sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    panel->SetSizer(sizer);

    // Eventy
    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_OK); }, wxID_OK);
    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    // Wyœrodkuj dialog na ekranie
    dlg.Centre();

    // Poka¿ dialog
    dlg.ShowModal();

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_OK); }, wxID_OK);
    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    if (dlg.ShowModal() == wxID_OK) {
        wxMessageBox("Account created successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
}