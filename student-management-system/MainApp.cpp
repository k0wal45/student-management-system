#include "MainApp.h"

wxIMPLEMENT_APP(MainApp); 
bool MainApp::OnInit()
{
	// Stworzenie g��wnego okna aplikacji
	MainFrame* frame = new MainFrame("Student Management System"); 
	// Ustawienie rozmiaru okna
	frame->SetClientSize(800, 600); 
	// Wy�rodkowanie okna na ekranie
	frame->Center(); 
	// Wy�wietlenie okna
	frame->Show(true); 
    return true;
}