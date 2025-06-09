#pragma once
#include <wx/wx.h>
#include <wx/valtext.h>

class LoginPanel : public wxPanel
{
public:

    //konstruktor
    LoginPanel(wxWindow* parent);

private:

    //pole tekstowe do wprowadzenia adresu e-mail
	wxTextCtrl* emailCtrl;

	//pole tekstowe do wprowadzenia has�a
    wxTextCtrl* passwordCtrl;

	//wyb�r roli u�ytkownika
    wxChoice* roleChoice;

    //obs�uga logowania
    void OnLogin(wxCommandEvent& event);
};
