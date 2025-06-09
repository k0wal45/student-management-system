//pliki nag��wkowe
#include "StudentPanel.h"
#include "MainFrame.h"
#include "Students.h"
#include "Teacher.h"
#include "Exam.h"
//WxWidgets
#include <wx/listctrl.h>
#include <wx/datectrl.h>


//Konstruktor
StudentPanel::StudentPanel(wxWindow* parent, const wxString& studentEmail)
    : wxPanel(parent), studentEmail(studentEmail)
{
    SetBackgroundColour(wxColour(240, 240, 240));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);


    //Wczytaj dane student�w
    vector<Students> allStudents = Students::loadStudentsFromFile();


    //Znajd� studenta na podstawie adresu e-mail
    wxString emailToFind = studentEmail;
    auto it = std::find_if(allStudents.begin(), allStudents.end(), [&emailToFind](const Students& student) {
        return student.email == emailToFind.ToStdString();
        });

    wxString headerText;

    if (it != allStudents.end()) {
        //Je�li student zosta� znaleziony, ustaw imi� i nazwisko w nag��wku
        const Students& student = *it;
        headerText = "Student Panel: " + wxString::FromUTF8(student.first_name + " " + student.last_name);
    }
    else {
        // Je�li nie znaleziono studenta, przekieruj do panelu logowania
        wxMessageBox("Student not found: " + studentEmail, "Error", wxOK | wxICON_ERROR);
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        if (frame) {
            frame->ShowLoginPanel();
        }

        //Zako�cz konstruktor, aby nie inicjalizowa� panelu
        return; 
    }

    //Nag��wek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, headerText,
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    //Notebook z zak�adkami
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    //Zak�adka Oceny
    wxPanel* gradesPanel = new wxPanel(notebook);
    wxBoxSizer* gradesSizer = new wxBoxSizer(wxVERTICAL);

    gradesList = new wxListCtrl(gradesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    gradesList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Grade", wxLIST_FORMAT_LEFT, 80);
    gradesList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    gradesList->AppendColumn("Comment", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Teacher", wxLIST_FORMAT_LEFT, 150);
    gradesSizer->Add(gradesList, 1, wxALL | wxEXPAND, 10);
    gradesPanel->SetSizer(gradesSizer);
    notebook->AddPage(gradesPanel, "Grades");

    //Zak�adka Sprawdziany
    wxPanel* examsPanel = new wxPanel(notebook);
    wxBoxSizer* examsSizer = new wxBoxSizer(wxVERTICAL);

    examsList = new wxListCtrl(examsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    examsList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    examsList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    examsList->AppendColumn("Description", wxLIST_FORMAT_LEFT, 250);
    examsList->AppendColumn("Teacher", wxLIST_FORMAT_LEFT, 150);
    ShowExams();
    examsSizer->Add(examsList, 1, wxALL | wxEXPAND, 10);
    examsPanel->SetSizer(examsSizer);
    notebook->AddPage(examsPanel, "Exams");

    //Dodaj notebook do g��wnego sizera
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


    //Za�aduj dane na temat studenta
    ShowStudentDetails();
}




//Wy�wietlenie danych studenta

void StudentPanel::ShowStudentDetails()
{
    if (!gradesList) return;

    gradesList->Freeze();
    gradesList->DeleteAllItems();


	//Wy�wietl oceny studenta oraz nauczycieli kt�rzy je wystawili
    try {
        vector<Students> allStudents = Students::loadStudentsFromFile();
        vector<Grade> allGrades = Grade::loadGradesFromFile();
        vector<Teacher> allTeachers = Teacher::loadTeachersFromFile();

        //Znajd� zalogowanego studenta na podstawie emaila
        auto it = std::find_if(allStudents.begin(), allStudents.end(), [this](const Students& student) {
            return (student.email) == studentEmail.ToStdString();
            });

        if (it != allStudents.end()) {
            const Students& student = *it;

            //Iteruj przez oceny studenta
            for (const auto& gradeId : student.grades) {

                // Znajd� ocen� na podstawie jej ID
                auto gradeIt = std::find_if(allGrades.begin(), allGrades.end(), [&gradeId](const Grade& grade){ 
                        return grade.id == gradeId; 
                    });


                if (gradeIt != allGrades.end()) {
                    const Grade& grade = *gradeIt;

                    //Znajd� nauczyciela, kt�ry wystawi� ocen�
                    wxString teacherName = "Unknown";
                    auto teacherIt = std::find_if(allTeachers.begin(), allTeachers.end(),[&gradeId](const Teacher& teacher) {

                            // Sprawd�, czy ID oceny znajduje si� w li�cie ocen nauczyciela
                            return std::find(teacher.grades.begin(), teacher.grades.end(), gradeId) != teacher.grades.end();

                        });

                    if (teacherIt != allTeachers.end()) {
                        const Teacher& teacher = *teacherIt;
                        teacherName = wxString::FromUTF8(teacher.first_name + " " + teacher.last_name);
                    }

                    //Dodaj dane do listy
                    long index = gradesList->InsertItem(gradesList->GetItemCount(), grade.subject);
                    gradesList->SetItem(index, 1, wxString::Format("%.2f", grade.grade));
                    gradesList->SetItem(index, 2, grade.date);
                    gradesList->SetItem(index, 3, grade.comment);
                    gradesList->SetItem(index, 4, teacherName); 
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




//Wy�wietlenie egzamin�w studenta

void StudentPanel::ShowExams()
{

    if (!examsList) return;

    examsList->Freeze();
    examsList->DeleteAllItems();

	//Wy�wietl sprawdziany studenta oraz nauczycieli kt�rzy je stworzyli
    try {
        vector<Exam> allExams = Exam::loadExamsFromFile();
        vector<Teacher> allTeachers = Teacher::loadTeachersFromFile();

        //Iteruj przez wszystkie sprawdziany
        for (const auto& exam : allExams) {
            wxString teacherName = "Unknown";

            //Usu� prefiks "t" z teacher_id, aby dopasowa� do id nauczyciela
            std::string teacherId = exam.teacher_id.substr(1); // Usuwa pierwszy znak (np. "t1" -> "1")

            //Znajd� nauczyciela na podstawie id
            auto teacherIt = std::find_if(allTeachers.begin(), allTeachers.end(),
                [&teacherId](const Teacher& teacher) {
                    return teacher.id == teacherId;
                });

            if (teacherIt != allTeachers.end()) {
                const Teacher& teacher = *teacherIt;
                teacherName = wxString::FromUTF8(teacher.first_name + " " + teacher.last_name);
            }

            //Dodaj dane sprawdzianu do listy
            long index = examsList->InsertItem(examsList->GetItemCount(), wxString::FromUTF8(exam.subject));
            examsList->SetItem(index, 1, wxString::FromUTF8(exam.date));
            examsList->SetItem(index, 2, wxString::FromUTF8(exam.comment));
            examsList->SetItem(index, 3, teacherName);
        }
    }
    catch (const std::exception& e) {
        wxMessageBox("Error loading exam data: " + wxString(e.what()),
            "Error", wxOK | wxICON_ERROR);
    }

    examsList->Thaw();
    examsList->Refresh();
}