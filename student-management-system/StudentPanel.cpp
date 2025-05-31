#include "StudentPanel.h"
#include "MainFrame.h"
#include <wx/listctrl.h>
#include <wx/datectrl.h>

StudentPanel::StudentPanel(wxWindow* parent, const wxString& studentName)
    : wxPanel(parent), studentName(studentName) 
{
    SetBackgroundColour(wxColour(240, 240, 240));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Nag³ówek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Student Panel: " + studentName, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    // Notebook z zak³adkami
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    // Zak³adka Oceny
    wxPanel* gradesPanel = new wxPanel(notebook);
    wxBoxSizer* gradesSizer = new wxBoxSizer(wxVERTICAL);

    wxListCtrl* gradesList = new wxListCtrl(gradesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    gradesList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Grade", wxLIST_FORMAT_LEFT, 80);
    gradesList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    gradesList->AppendColumn("Teacher", wxLIST_FORMAT_LEFT, 150);

    // Przyk³adowe oceny
    long index = gradesList->InsertItem(0, "Mathematics");
    gradesList->SetItem(index, 1, "A");
    gradesList->SetItem(index, 2, "2023-10-15");
    gradesList->SetItem(index, 3, "John Doe");

    index = gradesList->InsertItem(1, "Physics");
    gradesList->SetItem(index, 1, "B");
    gradesList->SetItem(index, 2, "2023-10-16");
    gradesList->SetItem(index, 3, "Jane Smith");

    gradesSizer->Add(gradesList, 1, wxALL | wxEXPAND, 5);
    gradesPanel->SetSizer(gradesSizer);

    // Zak³adka Sprawdziany
    wxPanel* examsPanel = new wxPanel(notebook);
    wxBoxSizer* examsSizer = new wxBoxSizer(wxVERTICAL);

    wxListCtrl* examsList = new wxListCtrl(examsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    examsList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    examsList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    examsList->AppendColumn("Description", wxLIST_FORMAT_LEFT, 250);
    examsList->AppendColumn("Teacher", wxLIST_FORMAT_LEFT, 150);

    // Przyk³adowe sprawdziany
    index = examsList->InsertItem(0, "Mathematics");
    examsList->SetItem(index, 1, "2023-11-15");
    examsList->SetItem(index, 2, "Midterm exam - chapters 1-5");
    examsList->SetItem(index, 3, "John Doe");

    index = examsList->InsertItem(1, "Physics");
    examsList->SetItem(index, 1, "2023-11-20");
    examsList->SetItem(index, 2, "Final exam - all chapters");
    examsList->SetItem(index, 3, "Jane Smith");

    examsSizer->Add(examsList, 1, wxALL | wxEXPAND, 5);
    examsPanel->SetSizer(examsSizer);

    notebook->AddPage(gradesPanel, "Grades");
    notebook->AddPage(examsPanel, "Exams");

    mainSizer->Add(notebook, 1, wxALL | wxEXPAND, 5);

    // Przycisk powrotu
    wxButton* backBtn = new wxButton(this, wxID_ANY, "Back to Login");
    backBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        frame->ShowLoginPanel();
        });

    mainSizer->Add(backBtn, 0, wxALL | wxALIGN_RIGHT, 5);
    SetSizer(mainSizer);
}