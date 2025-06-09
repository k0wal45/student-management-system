#pragma once
#include "MainFrame.h"
#include <wx/wx.h>

class MainApp : public wxApp
{
public:

	// Inicjalizacja aplikacji
	virtual bool OnInit(); 

};

wxDECLARE_APP(MainApp);

