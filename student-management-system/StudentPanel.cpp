#include "StudentPanel.h"
#include "MainFrame.h"
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include "Students.h"
StudentPanel::StudentPanel(wxWindow* parent, const wxString& studentEmail)
    : wxPanel(parent), studentEmail(studentEmail)
{
    SetBackgroundColour(wxColour(240, 240, 240));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);


    // Wczytaj dane studentów
    vector<Students> allStudents = Students::loadStudentsFromFile();


    // ZnajdŸ studenta na podstawie adresu e-mail


    wxString emailToFind = studentEmail; // Przypisz wartoœæ do zmiennej lokalnej
    auto it = std::find_if(allStudents.begin(), allStudents.end(), [&emailToFind](const Students& student) {
        return student.email == emailToFind.ToStdString();
        });

    wxString headerText;
    if (it != allStudents.end()) {
        // Jeœli student zosta³ znaleziony, ustaw imiê i nazwisko w nag³ówku
        const Students& student = *it;
        headerText = "Student Panel: " + wxString::FromUTF8(student.first_name + " " + student.last_name);
    }
    else {
        // Jeœli nie znaleziono studenta, przekieruj do panelu logowania
        wxMessageBox("Student not found: " + studentEmail, "Error", wxOK | wxICON_ERROR);
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        if (frame) {
            frame->ShowLoginPanel();
        }
        return; // Zakoñcz konstruktor, aby nie inicjalizowaæ panelu
    }

    // Nag³ówek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, headerText,
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    // Notebook z zak³adkami
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    // Zak³adka Oceny
    wxPanel* gradesPanel = new wxPanel(notebook);
    wxBoxSizer* gradesSizer = new wxBoxSizer(wxVERTICAL);

    gradesList = new wxListCtrl(gradesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    gradesList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Grade", wxLIST_FORMAT_LEFT, 80);
    gradesList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    gradesList->AppendColumn("Comment", wxLIST_FORMAT_LEFT, 150);

    gradesSizer->Add(gradesList, 1, wxALL | wxEXPAND, 10);
    gradesPanel->SetSizer(gradesSizer);
    notebook->AddPage(gradesPanel, "Grades");

    // Zak³adka Sprawdziany
    wxPanel* examsPanel = new wxPanel(notebook);
    wxBoxSizer* examsSizer = new wxBoxSizer(wxVERTICAL);

    examsList = new wxListCtrl(examsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    examsList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    examsList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    examsList->AppendColumn("Description", wxLIST_FORMAT_LEFT, 250);
    examsList->AppendColumn("Teacher", wxLIST_FORMAT_LEFT, 150);

    examsSizer->Add(examsList, 1, wxALL | wxEXPAND, 10);
    examsPanel->SetSizer(examsSizer);
    notebook->AddPage(examsPanel, "Exams");

    // Dodaj notebook do g³ównego sizera
    mainSizer->Add(notebook, 1, wxALL | wxEXPAND, 10);

    // Przycisk powrotu
    wxButton* backBtn = new wxButton(this, wxID_ANY, "Back to Login");
    backBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        if (frame) frame->ShowLoginPanel();
        });
    mainSizer->Add(backBtn, 0, wxALL | wxALIGN_RIGHT, 5);

    SetSizer(mainSizer);
    Layout();

    // Za³aduj dane PO pe³nej inicjalizacji interfejsu
    ShowStudentDetails();
}

void StudentPanel::ShowStudentDetails()
{
    if (!gradesList) return;

    gradesList->Freeze();
    gradesList->DeleteAllItems();

    try {
        vector<Students> allStudents = Students::loadStudentsFromFile();
        vector<Grade> allGrades = Grade::loadGradesFromFile();

        auto it = std::find_if(allStudents.begin(), allStudents.end(), [this](const Students& student) {
            return (student.email) == studentEmail.ToStdString();
            });

        if (it != allStudents.end()) {
            const Students& student = *it;

            for (const auto& gradeId : student.grades) {
                auto gradeIt = std::find_if(allGrades.begin(), allGrades.end(),
                    [&gradeId](const Grade& grade) { return grade.id == gradeId; });

                if (gradeIt != allGrades.end()) {
                    const Grade& grade = *gradeIt;
                    long index = gradesList->InsertItem(gradesList->GetItemCount(), grade.subject);
                    gradesList->SetItem(index, 1, wxString::Format("%.2f", grade.grade));
                    gradesList->SetItem(index, 2, grade.date);
                    gradesList->SetItem(index, 3, grade.comment);
                }
            }
        }
        else {
            wxMessageBox("Student not found: " + studentEmail, "Error", wxOK | wxICON_ERROR);
        }
    }
    catch (const std::exception& e) {
        wxMessageBox("Error loading student data: " + wxString(e.what()),
            "Error", wxOK | wxICON_ERROR);
    }

    gradesList->Thaw();
    gradesList->Refresh();
}