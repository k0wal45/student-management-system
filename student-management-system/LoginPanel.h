#pragma once
//panel logowania u�ytkownika
#include <wx/wx.h>
#include <wx/valtext.h>

class LoginPanel : public wxPanel
{
public:
    LoginPanel(wxWindow* parent);

private:
    // pole tekstowe do wprowadzenia adresu e-mail
	wxTextCtrl* emailCtrl;

	// pole tekstowe do wprowadzenia has�a
    wxTextCtrl* passwordCtrl;

	// wyb�r roli u�ytkownika
    wxChoice* roleChoice;

    //zalogowanie
    void OnLogin(wxCommandEvent& event);

	//rejestracja
    void OnRegister(wxCommandEvent& event);

	//wy�wietlenie okna rejestracji
    void ShowRegistrationDialog();

    void ShowPasswordResetDialog(wxCommandEvent& event);
};
