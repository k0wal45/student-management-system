#include "MainApp.h"

wxIMPLEMENT_APP(MainApp); 
bool MainApp::OnInit()
{
	//Stworzenie głównego okna aplikacji
	MainFrame* frame = new MainFrame("Student Management System"); 

	//Ustawienie rozmiaru okna
	frame->SetClientSize(800, 600); 

	//Wyśrodkowanie okna na ekranie
	frame->Center(); 

	//Wyświetlenie okna
	frame->Show(true); 

    return true;
}