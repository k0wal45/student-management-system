#include "TeacherPanel.h"
#include "MainFrame.h"
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
#include "Students.h"
#include "TeacherPanel.h"
#include "MainFrame.h"
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include "Students.h"
#include "Grade.h"
#include <set>

TeacherPanel::TeacherPanel(wxWindow* parent, const wxString& teacherName)
    : wxPanel(parent), teacherName(teacherName), notebook(nullptr)
{
    SetBackgroundColour(wxColour(240, 240, 240));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSize(800, 600);

    // Nag³ówek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Teacher Panel: " + teacherName, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    // Panel wyszukiwania
    wxPanel* searchPanel = new wxPanel(this);
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);

    searchCtrl = new wxTextCtrl(searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    searchCtrl->SetHint("Wpisz imiê ucznia...");
    wxButton* searchBtn = new wxButton(searchPanel, wxID_ANY, "Szukaj");

    searchSizer->Add(searchCtrl, 0, wxALL, 5);
    searchSizer->Add(searchBtn, 0, wxALL, 5);
    searchPanel->SetSizer(searchSizer);

    // Notebook z zak³adkami
    notebook = new wxNotebook(this, wxID_ANY);

    // Zak³adka Studenci
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);

    // Inicjalizacja listy studentów - teraz jako member klasy
    studentsList = new wxListCtrl(studentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    studentsList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    studentsList->AppendColumn("First Name", wxLIST_FORMAT_LEFT, 100);
    studentsList->AppendColumn("Last Name", wxLIST_FORMAT_LEFT, 100);
    studentsList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);
    studentsList->AppendColumn("Major", wxLIST_FORMAT_LEFT, 150);
    studentsList->AppendColumn("Year", wxLIST_FORMAT_LEFT, 50);

    // Wczytanie i wyœwietlenie studentów z pliku JSON
    vector<Students> students = Students::loadStudentsFromFile();
    for (const Students& student : students) {
        long index = studentsList->InsertItem(studentsList->GetItemCount(), student.id);
        studentsList->SetItem(index, 1, student.first_name);
        studentsList->SetItem(index, 2, student.last_name);
        studentsList->SetItem(index, 3, student.email);
        studentsList->SetItem(index, 4, student.major);
        studentsList->SetItem(index, 5, wxString::Format("%d", student.year));
    }

    studentsList->SetBackgroundColour(wxColour(255, 255, 255));
    studentsList->SetTextColour(wxColour(0, 0, 0));
    studentsList->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    studentsSizer->Add(studentsList, 1, wxALL | wxEXPAND, 5);
    studentsPanel->SetSizer(studentsSizer);

    // Zak³adka Oceny
    wxPanel* gradesPanel = new wxPanel(notebook);
    wxBoxSizer* gradesSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addGradeBtn = new wxButton(gradesPanel, wxID_ANY, "Add Grade");
    addGradeBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnAddGrade, this);

    wxButton* removeGradeBtn = new wxButton(gradesPanel, wxID_ANY, "Remove Grade");
    removeGradeBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnRemoveGrade, this);


    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL); // Sizer dla przycisków
    buttonSizer->Add(addGradeBtn, 0, wxALL, 5);
    buttonSizer->Add(removeGradeBtn, 0, wxALL, 5);

   

    gradesList = new wxListCtrl(gradesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    
    gradesList->AppendColumn("Student", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Grade", wxLIST_FORMAT_LEFT, 80);
    gradesList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    gradesList->AppendColumn("Comment", wxLIST_FORMAT_LEFT, 200);
   

    // Wczytanie i wyœwietlenie ocen z pliku JSON
    vector<Grade> allGrades = Grade::loadGradesFromFile();
    vector<Students> allStudents = Students::loadStudentsFromFile();

    for (const Grade& grade : allGrades) {
        // ZnajdŸ studenta, który ma tê ocenê
        for (const Students& student : allStudents) {
            if (std::find(student.grades.begin(), student.grades.end(), grade.id) != student.grades.end()) {
                long index = gradesList->InsertItem(gradesList->GetItemCount(),
                    student.first_name + " " + student.last_name);
                gradesList->SetItem(index, 1, grade.subject);
                gradesList->SetItem(index, 2, wxString::Format("%.1f", grade.grade));
                gradesList->SetItem(index, 3, grade.date);
                gradesList->SetItem(index, 4, grade.comment);
                break;
            }
        }
    }
    gradesSizer->Add(buttonSizer, 0, wxALIGN_LEFT | wxALL, 5); // Dodanie sizeru przycisków do g³ównego sizeru
    gradesSizer->Add(gradesList, 1, wxALL | wxEXPAND, 5);
    gradesPanel->SetSizer(gradesSizer);

    // Zak³adka Sprawdziany
    wxPanel* examsPanel = new wxPanel(notebook);
    wxBoxSizer* examsSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addExamBtn = new wxButton(examsPanel, wxID_ANY, "Add Exam");
    addExamBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnAddExam, this);

    wxListCtrl* examsList = new wxListCtrl(examsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    examsList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    examsList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    examsList->AppendColumn("Description", wxLIST_FORMAT_LEFT, 250);

    // Przyk³adowe sprawdziany
    long index = examsList->InsertItem(0, "Mathematics");
    examsList->SetItem(index, 1, "2023-11-15");
    examsList->SetItem(index, 2, "Midterm exam - chapters 1-5");

    index = examsList->InsertItem(1, "Physics");
    examsList->SetItem(index, 1, "2023-11-20");
    examsList->SetItem(index, 2, "Final exam - all chapters");

    examsSizer->Add(addExamBtn, 0, wxALL, 5);
    examsSizer->Add(examsList, 1, wxALL | wxEXPAND, 5);
    examsPanel->SetSizer(examsSizer);

    notebook->AddPage(studentsPanel, "Students");
    notebook->AddPage(gradesPanel, "Grades");
    notebook->AddPage(examsPanel, "Exams");

    mainSizer->Add(searchPanel, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(notebook, 1, wxALL | wxEXPAND, 5);
    // Eventy
    searchBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnSearch, this);
    searchCtrl->Bind(wxEVT_TEXT, &TeacherPanel::OnSearch, this); // Wyszukiwanie podczas wpisywania
    // Przycisk powrotu
    wxButton* backBtn = new wxButton(this, wxID_ANY, "Back to Login");
    backBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        frame->ShowLoginPanel();
        });

    mainSizer->Add(backBtn, 0, wxALL | wxALIGN_RIGHT, 5);
    SetSizer(mainSizer);
    
    
}

void TeacherPanel::OnSearch(wxCommandEvent& event)
{
    wxString searchText = searchCtrl->GetValue();
    int activeTab = notebook->GetSelection();

    if (activeTab == 0) { // Zak³adka Studenci
        FilterData(searchText, false);
    }
    else if (activeTab == 1) { // Zak³adka Oceny
        FilterData(searchText, true);
    }
}


void TeacherPanel::FilterData(const wxString& searchText, bool filterGrades)
{
    wxBusyCursor busyCursor;

    if (filterGrades) {
        if (!gradesList) return;

        gradesList->Freeze();
        gradesList->DeleteAllItems();

        try {
            vector<Grade> allGrades = Grade::loadGradesFromFile();
            vector<Students> allStudents = Students::loadStudentsFromFile();
            wxString searchLower = searchText.Lower();

            for (const Grade& grade : allGrades) {
                for (const Students& student : allStudents) {
                    if (std::find(student.grades.begin(), student.grades.end(), grade.id) != student.grades.end()) {
                        wxString studentName = student.first_name + " " + student.last_name;
                        wxString subject = grade.subject;
                        wxString comment = grade.comment;

                        bool match = searchText.IsEmpty() ||
                            studentName.Lower().Contains(searchLower) ||
                            subject.Lower().Contains(searchLower) ||
                            comment.Lower().Contains(searchLower);

                        if (match) {
                            long index = gradesList->InsertItem(gradesList->GetItemCount(), studentName);
                            gradesList->SetItem(index, 1, grade.subject);
                            gradesList->SetItem(index, 2, wxString::Format("%.1f", grade.grade));
                            gradesList->SetItem(index, 3, grade.date);
                            gradesList->SetItem(index, 4, grade.comment);
                        }
                    }
                }
            }
        }
        catch (const exception& e) {
            wxLogError("Error during filtering grades: %s", e.what());
        }
        catch (...) {
            wxLogError("Unknown error during filtering grades");
        }

        gradesList->Thaw();
        gradesList->Refresh();
    }
    else {
        if (!studentsList) return;

        studentsList->Freeze();
        studentsList->DeleteAllItems();

        try {
            vector<Students> allStudents = Students::loadStudentsFromFile();
            wxString searchLower = searchText.Lower();

            for (const Students& student : allStudents) {
                wxString name = student.first_name;
                wxString surname = student.last_name;

                bool match = searchText.IsEmpty() ||
                    name.Lower().Contains(searchLower) ||
                    surname.Lower().Contains(searchLower);

                if (match) {
                    long index = studentsList->InsertItem(studentsList->GetItemCount(), student.id);
                    studentsList->SetItem(index, 1, student.first_name);
                    studentsList->SetItem(index, 2, student.last_name);
                    studentsList->SetItem(index, 3, student.email);
                    studentsList->SetItem(index, 4, student.major);
                    studentsList->SetItem(index, 5, wxString::Format("%d", student.year));
                }
            }
        }
        catch (const exception& e) {
            wxLogError("Error during filtering students: %s", e.what());
        }
        catch (...) {
            wxLogError("Unknown error during filtering students");
        }

        studentsList->Thaw();
        studentsList->Refresh();
    }
}

void TeacherPanel::OnAddGrade(wxCommandEvent& event)
{
    ShowAddGradeDialog();
}
void TeacherPanel::OnRemoveGrade(wxCommandEvent& event)
{
    ShowRemoveGradeDialog();
}
void TeacherPanel::OnAddExam(wxCommandEvent& event)
{
    ShowAddExamDialog();
}
void TeacherPanel::ShowRemoveGradeDialog() {
    wxDialog* removeGradeDialog = new wxDialog(this, wxID_ANY, "Remove Grade", wxDefaultPosition, wxSize(1000, 700));
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

    // Pole wyboru studenta
    wxStaticText* studentLabel = new wxStaticText(removeGradeDialog, wxID_ANY, "Student:");
    wxComboBox* studentComboBox = new wxComboBox(removeGradeDialog, wxID_ANY);
    vector<Students> allStudents = Students::loadStudentsFromFile();
    for (const Students& student : allStudents) {
        studentComboBox->Append(student.first_name + " " + student.last_name);
    }

    // Pole wyboru przedmiotu
    wxStaticText* subjectLabel = new wxStaticText(removeGradeDialog, wxID_ANY, "Subject:");
    wxTextCtrl* subjectTextCtrl = new wxTextCtrl(removeGradeDialog, wxID_ANY);

    // Pole oceny
    wxStaticText* gradeLabel = new wxStaticText(removeGradeDialog, wxID_ANY, "Grade:");
    wxTextCtrl* gradeTextCtrl = new wxTextCtrl(removeGradeDialog, wxID_ANY);

    // Przyciski
    wxButton* removeButton = new wxButton(removeGradeDialog, wxID_OK, "Remove");
    wxButton* cancelButton = new wxButton(removeGradeDialog, wxID_CANCEL, "Cancel");

    // Uk³ad elementów w dialogu
    wxBoxSizer* studentSizer = new wxBoxSizer(wxVERTICAL);
    studentSizer->Add(studentLabel, 0, wxRIGHT, 5);
    studentSizer->Add(studentComboBox, 1, wxEXPAND);

    wxBoxSizer* subjectSizer = new wxBoxSizer(wxVERTICAL);
    subjectSizer->Add(subjectLabel, 0, wxRIGHT, 5);
    subjectSizer->Add(subjectTextCtrl, 1, wxEXPAND);

    wxBoxSizer* gradeSizer = new wxBoxSizer(wxVERTICAL);
    gradeSizer->Add(gradeLabel, 0, wxRIGHT, 5);
    gradeSizer->Add(gradeTextCtrl, 1, wxEXPAND);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(removeButton, 0, wxRIGHT, 5);
    buttonSizer->Add(cancelButton, 0, wxLEFT, 5);

    dialogSizer->Add(studentSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(subjectSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(gradeSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    removeGradeDialog->SetSizer(dialogSizer);
    dialogSizer->Fit(removeGradeDialog);

    if (removeGradeDialog->ShowModal() == wxID_OK) {
        // Pobierz dane z pól
        wxString studentName = studentComboBox->GetValue();
        wxString subject = subjectTextCtrl->GetValue();
        double grade = wxAtof(gradeTextCtrl->GetValue());

        // Usuñ ocenê od odpowiedniego studenta
        for (Students& student : allStudents) {
            if (studentName == student.first_name + " " + student.last_name) {
                vector<string>& studentGrades = student.grades;
                vector<Grade> allGrades = Grade::loadGradesFromFile();

                auto it = std::find_if(allGrades.begin(), allGrades.end(), [&](const Grade& g) {
                    return g.subject == subject.ToStdString() && g.grade == grade;
                });

                if (it != allGrades.end()) {
                    student.removeGrade(it->id);
                    allGrades.erase(it);

                    // Zapisz zaktualizowan¹ listê ocen
                    Grade::saveGradesToFile(allGrades);

                    // Zapisz zaktualizowan¹ listê studentów
                    Students::saveStudentsToFile(allStudents);

                    // Odœwie¿ listê ocen
                    RefreshGradesList();
                    wxMessageBox("Grade removed successfully!", "Success", wxOK | wxICON_INFORMATION);
                } else {
                    wxMessageBox("Grade not found!", "Error", wxOK | wxICON_ERROR);
                }
                break;
            }
        }
    }

    removeGradeDialog->Destroy();
}
void TeacherPanel::ShowAddGradeDialog()
{
    wxDialog* addGradeDialog = new wxDialog(this, wxID_ANY, "Add Grade", wxDefaultPosition, wxSize(1000, 700));
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

    // Pole wyboru studenta
    wxStaticText* studentLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Student:");
    wxComboBox* studentComboBox = new wxComboBox(addGradeDialog, wxID_ANY);
    vector<Students> allStudents = Students::loadStudentsFromFile();
    for (const Students& student : allStudents) {
        studentComboBox->Append(student.first_name + " " + student.last_name);
    }

    // Pole wyboru przedmiotu
    wxStaticText* subjectLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Subject:");
    wxComboBox* subjectComboBox = new wxComboBox(addGradeDialog, wxID_ANY);
    vector<Grade> allGrades = Grade::loadGradesFromFile();
    set<string> subjects; // U¿ywamy `set`, aby unikn¹æ duplikatów
    for (const Grade& grade : allGrades) {
        subjects.insert(grade.subject);
    }
    for (const string& subject : subjects) {
        subjectComboBox->Append(subject);
    }

    // Label i lista rozwijalna dla wyboru oceny
    wxStaticText* gradeLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Grade:");
    wxComboBox* gradeComboBox = new wxComboBox(addGradeDialog, wxID_ANY);
    set<double> grades; // U¿ywamy `set`, aby unikn¹æ duplikatów
    for (const Grade& grade : allGrades) {
        grades.insert(grade.grade);
    }
    for (double gradeValue : grades) {
        gradeComboBox->Append(wxString::Format("%.1f", gradeValue));
    }

    // Pole komentarza
    wxStaticText* commentLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Comment:");
    wxTextCtrl* commentTextCtrl = new wxTextCtrl(addGradeDialog, wxID_ANY);

    // Przyciski
    wxButton* saveButton = new wxButton(addGradeDialog, wxID_OK, "Save");
    wxButton* cancelButton = new wxButton(addGradeDialog, wxID_CANCEL, "Cancel");

    // Uk³ad elementów w dialogu
   // Uk³ad elementów w dialogu
    wxBoxSizer* studentSizer = new wxBoxSizer(wxVERTICAL);
    studentSizer->Add(studentLabel, 0, wxCENTER, 10); // Zwiêkszenie marginesu
    studentSizer->Add(studentComboBox, 1, wxEXPAND);

    wxBoxSizer* subjectSizer = new wxBoxSizer(wxVERTICAL);
    subjectSizer->Add(subjectLabel, 0, wxCENTER, 10); // Zwiêkszenie marginesu
    subjectSizer->Add(subjectComboBox, 1, wxEXPAND);

    wxBoxSizer* gradeSizer = new wxBoxSizer(wxVERTICAL);
    gradeSizer->Add(gradeLabel, 0, wxCENTER, 10); // Zwiêkszenie marginesu
    gradeSizer->Add(gradeComboBox, 1, wxEXPAND);

    wxBoxSizer* commentSizer = new wxBoxSizer(wxVERTICAL);
    commentSizer->Add(commentLabel, 0, wxCENTER, 10); // Zwiêkszenie marginesu
    commentSizer->Add(commentTextCtrl, 1, wxEXPAND);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(saveButton, 0, wxRIGHT, 10); // Zwiêkszenie marginesu
    buttonSizer->Add(cancelButton, 0, wxLEFT, 10);

    dialogSizer->Add(studentSizer, 0, wxALL | wxEXPAND, 10); // Zwiêkszenie marginesów
    dialogSizer->Add(subjectSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(gradeSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(commentSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    addGradeDialog->SetSizer(dialogSizer);
    dialogSizer->Fit(addGradeDialog);
    if (addGradeDialog->ShowModal() == wxID_OK) {
        // Pobierz dane z pól
        wxString studentName = studentComboBox->GetValue();
        wxString subject = subjectComboBox->GetValue();
        double grade = wxAtof(gradeComboBox->GetValue());
        wxString comment = commentTextCtrl->GetValue();

        // Dodaj ocenê do odpowiedniego studenta
        for (Students& student : allStudents) {
            if (studentName == student.first_name + " " + student.last_name) {
                Grade newGrade("generated_id", grade, subject.ToStdString(), comment.ToStdString(), wxDateTime::Now().FormatISODate().ToStdString());
                student.addGrade(newGrade.id);

                // Zapisz ocenê do pliku JSON
                vector<Grade> grades = Grade::loadGradesFromFile();
                grades.push_back(newGrade);
                Grade::saveGradesToFile(grades);

                // Zapisz zaktualizowan¹ listê studentów
                Students::saveStudentsToFile(allStudents);

                // Odœwie¿ listê ocen
                RefreshGradesList();
                break;
            }
        }
    }

    addGradeDialog->Destroy();
}

void TeacherPanel::RefreshGradesList()
{
    if (!gradesList) return;

    gradesList->Freeze();
    gradesList->DeleteAllItems();

    vector<Grade> allGrades = Grade::loadGradesFromFile();
    vector<Students> allStudents = Students::loadStudentsFromFile();

    for (const Grade& grade : allGrades) {
        // ZnajdŸ studenta dla tej oceny
        for (const Students& student : allStudents) {
            if (std::find(student.grades.begin(), student.grades.end(), grade.id) != student.grades.end()) {
                long index = gradesList->InsertItem(gradesList->GetItemCount(),
                    wxString(student.first_name + " " + student.last_name));
                gradesList->SetItem(index, 1, grade.subject);
                gradesList->SetItem(index, 2, wxString::Format("%.1f", grade.grade));
                gradesList->SetItem(index, 3, grade.date);
                gradesList->SetItem(index, 4, grade.comment);
                break;
            }
        }
    }

    gradesList->Thaw();
    gradesList->Refresh();
}
void TeacherPanel::ShowAddExamDialog()
{
    wxDialog dlg(this, wxID_ANY, "Add New Exam", wxDefaultPosition, wxSize(400, 300));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Add New Exam");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxTextCtrl* subjectCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxDatePickerCtrl* dateCtrl = new wxDatePickerCtrl(panel, wxID_ANY);
    wxTextCtrl* descriptionCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    wxFlexGridSizer* inputSizer = new wxFlexGridSizer(2, 5, 10);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Subject:"));
    inputSizer->Add(subjectCtrl);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Date:"));
    inputSizer->Add(dateCtrl);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Description:"));
    inputSizer->Add(descriptionCtrl);

    wxButton* addBtn = new wxButton(panel, wxID_OK, "Add");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(addBtn, 0, wxALL, 5);
    buttonSizer->Add(cancelBtn, 0, wxALL, 5);

    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 10);
    sizer->Add(inputSizer, 0, wxALL | wxALIGN_CENTER, 10);
    sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_OK); }, wxID_OK);
    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    if (dlg.ShowModal() == wxID_OK) {
        wxMessageBox("Exam added successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
}


