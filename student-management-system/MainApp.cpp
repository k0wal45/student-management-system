#include "MainApp.h"

wxIMPLEMENT_APP(MainApp); 
bool MainApp::OnInit()
{
	// Stworzenie g³ównego okna aplikacji
	MainFrame* frame = new MainFrame("Student Management System"); 
	// Ustawienie rozmiaru okna
	frame->SetClientSize(800, 600); 
	// Wyœrodkowanie okna na ekranie
	frame->Center(); 
	// Wyœwietlenie okna
	frame->Show(true); 
    return true;
}