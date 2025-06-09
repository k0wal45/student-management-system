//Pliki nag³ówkowe
#include "AdminPanel.h"
#include "StudentPanel.h"
#include "bcrypt.h"
#include "MainFrame.h"
#include "Students.h"
#include "Grade.h"
#include "Teacher.h"
#include "TeacherPanel.h"
//WxWidgets
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
//Biblioteki standardowe
#include <set>
#include <regex>
#include <algorithm>
#include <iterator>
#include <fstream>


//konstruktor panelu administratora

AdminPanel::AdminPanel(wxWindow* parent)
    : wxPanel(parent)
{
    SetBackgroundColour(wxColour(240, 240, 240));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    //Nag³ówek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Administrator Panel", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    //Notebook z zak³adkami
    notebook = new wxNotebook(this, wxID_ANY);

    //Zak³adka Studenci
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);

    //Przyciski dodawania i usuwania studentów
    wxBoxSizer* studentButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addStudentBtn = new wxButton(studentsPanel, wxID_ANY, "Add New Student");
    addStudentBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnAddStudent, this);

    wxButton* removeStudentBtn = new wxButton(studentsPanel, wxID_ANY, "Remove Student");
    removeStudentBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnRemoveStudent, this);

    studentButtonSizer->Add(addStudentBtn, 0, wxALL, 5);
    studentButtonSizer->Add(removeStudentBtn, 0, wxALL, 5);

    studentsSizer->Add(studentButtonSizer, 0, wxALL, 5);

    //Lista studentów
    wxListCtrl* studentsList = new wxListCtrl(studentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    studentsList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 80);
    studentsList->AppendColumn("First name", wxLIST_FORMAT_LEFT, 150);
    studentsList->AppendColumn("Last Name", wxLIST_FORMAT_LEFT, 150);
    studentsList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);
    studentsList->AppendColumn("Major", wxLIST_FORMAT_LEFT, 120);
    studentsList->AppendColumn("Year", wxLIST_FORMAT_LEFT, 80);

    vector<Students> students = Students::loadStudentsFromFile();

    for (const Students& student : students) {
        long index = studentsList->InsertItem(studentsList->GetItemCount(), student.id);
        studentsList->SetItem(index, 1, student.first_name);
        studentsList->SetItem(index, 2, student.last_name);
        studentsList->SetItem(index, 3, student.email);
        studentsList->SetItem(index, 4, student.major);
        studentsList->SetItem(index, 5, wxString::Format("%d", student.year));
    }
    studentsSizer->Add(studentsList, 1, wxALL | wxEXPAND, 5);
    studentsPanel->SetSizer(studentsSizer);

    notebook->AddPage(studentsPanel, "Students");

    //Zak³adka Nauczyciele
    wxPanel* teachersPanel = new wxPanel(notebook);
    wxBoxSizer* teachersSizer = new wxBoxSizer(wxVERTICAL);

    //Przyciski dodawania nauczycieli
    wxBoxSizer* teacherButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addTeacherBtn = new wxButton(teachersPanel, wxID_ANY, "Add New Teacher");
    addTeacherBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnAddTeacher, this);
    wxButton* removeTeacherBtn = new wxButton(teachersPanel, wxID_ANY, "Remove Teacher");
    removeTeacherBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnRemoveTeacher, this);

    teacherButtonSizer->Add(addTeacherBtn, 0, wxALL, 5);
    teacherButtonSizer->Add(removeTeacherBtn, 0, wxALL, 5);
    teachersSizer->Add(teacherButtonSizer, 0, wxALL, 5);

    //Lista nauczycieli
    wxListCtrl* teachersList = new wxListCtrl(teachersPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    teachersList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 80);
    teachersList->AppendColumn("First name", wxLIST_FORMAT_LEFT, 150);
    teachersList->AppendColumn("Last Name", wxLIST_FORMAT_LEFT, 150);
    teachersList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);
    teachersList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);

    //Wczytanie nauczycieli z pliku teachers.json
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();

    for (const Teacher& teacher : teachers) {
        long index = teachersList->InsertItem(teachersList->GetItemCount(), teacher.id);
        teachersList->SetItem(index, 1, teacher.first_name);
        teachersList->SetItem(index, 2, teacher.last_name);
        teachersList->SetItem(index, 3, teacher.email);
        teachersList->SetItem(index, 4, teacher.subject);
    }

    teachersSizer->Add(teachersList, 1, wxALL | wxEXPAND, 5);
    teachersPanel->SetSizer(teachersSizer);

    notebook->AddPage(teachersPanel, "Teachers");

    mainSizer->Add(notebook, 1, wxALL | wxEXPAND, 5);

    //Przycisk powrotu
    wxButton* backBtn = new wxButton(this, wxID_ANY, "Back to Login");
    backBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        frame->ShowLoginPanel();
        });

    mainSizer->Add(backBtn, 0, wxALL | wxALIGN_RIGHT, 5);
    SetSizer(mainSizer);
}



//Odœwie¿enie listy studentów

void AdminPanel::RefreshStudentList()
{
        if (!studentsList) return;

        studentsList->Freeze();
        studentsList->DeleteAllItems();

		//Wczytanie studentów z pliku students.json
        vector<Students> allStudents = Students::loadStudentsFromFile();

        for (const Students& student : allStudents) {
            long index = studentsList->InsertItem(studentsList->GetItemCount(), student.id);
            studentsList->SetItem(index, 1, student.first_name);
            studentsList->SetItem(index, 2, student.last_name);
            studentsList->SetItem(index, 3, student.email);
            studentsList->SetItem(index, 4, student.major);
            studentsList->SetItem(index, 5, wxString::Format("%d", student.year));
        }

        studentsList->Thaw();
        studentsList->Refresh();
}



//Odœwie¿enie listy nauczycieli

void AdminPanel::RefreshTeacherList()
{
    if (!teachersList) return;

    teachersList->Freeze();
    teachersList->DeleteAllItems();
	//Wczytanie nauczycieli z pliku teachers.json
    vector<Teacher> allTeachers = Teacher::loadTeachersFromFile();

    for (const Teacher& teacher : allTeachers) {
        long index = teachersList->InsertItem(teachersList->GetItemCount(), teacher.id);
        teachersList->SetItem(index, 1, teacher.first_name);
        teachersList->SetItem(index, 2, teacher.last_name);
        teachersList->SetItem(index, 3, teacher.email);
        teachersList->SetItem(index, 4, teacher.subject);
    }

    teachersList->Thaw();
    teachersList->Refresh();
}






//Usuniêcie studenta wraz z jego ocenami

void AdminPanel::OnRemoveStudent(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Remove Student", wxDefaultPosition, wxSize(300, 600));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	//Nag³ówek
    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Remove Student");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 10);

	//Wczytanie studentów z pliku students.json
    vector<Students> students = Students::loadStudentsFromFile();

    //Tworzenie listy rozwijalnej z pe³nymi nazwiskami studentów
    wxStaticText* studentLabel = new wxStaticText(panel, wxID_ANY, "Select Student:");
    sizer->Add(studentLabel, 0, wxALL | wxALIGN_CENTER, 5);

    wxChoice* studentChoice = new wxChoice(panel, wxID_ANY);
    for (const Students& student : students) {
        studentChoice->Append(student.first_name + " " + student.last_name);
    }
    sizer->Add(studentChoice, 0, wxALL | wxEXPAND, 5);

    //Przyciski
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* removeBtn = new wxButton(panel, wxID_OK, "Remove");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
    buttonSizer->Add(removeBtn, 0, wxALL, 5);
    buttonSizer->Add(cancelBtn, 0, wxALL, 5);

    sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&dlg, studentChoice, &students, this](wxCommandEvent&) {
        int selectedIndex = studentChoice->GetSelection();

        if (selectedIndex == wxNOT_FOUND) {
            wxMessageBox("Please select a student.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        //Pobranie wybranego studenta
        const Students& selectedStudent = students[selectedIndex];
        string studentId = selectedStudent.id;
        string studentEmail = selectedStudent.email;
        vector<string> gradesToRemove = selectedStudent.grades;

        //Usuniêcie studenta z listy
        students.erase(students.begin() + selectedIndex);
        Students::saveStudentsToFile(students);


        //Wczytanie i aktualizacja listy ocen
        vector<Grade> grades = Grade::loadGradesFromFile();
        grades.erase(remove_if(grades.begin(), grades.end(), [&gradesToRemove](const Grade& grade) {
            return find(gradesToRemove.begin(), gradesToRemove.end(), grade.id) != gradesToRemove.end();
            }), grades.end());
        Grade::saveGradesToFile(grades);


        //Wczytanie i aktualizacja nauczycieli
        vector<Teacher> teachers = Teacher::loadTeachersFromFile();
        for (Teacher& teacher : teachers) {
            teacher.grades.erase(remove_if(teacher.grades.begin(), teacher.grades.end(), [&gradesToRemove](const string& gradeId) {
                return find(gradesToRemove.begin(), gradesToRemove.end(), gradeId) != gradesToRemove.end();
                }), teacher.grades.end());
        }
        Teacher::saveTeachersToFile(teachers);

        //Usuniêcie u¿ytkownika z pliku `users.json`
        ifstream usersFile("users.json");
        json usersData;

        if (usersFile.is_open()) {
            usersFile >> usersData;
            usersFile.close();
        }
        else {
            wxMessageBox("Cannot open users.json file.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        auto userIt = find_if(usersData.begin(), usersData.end(), [&studentEmail](const json& user) {
            return user["login"] == studentEmail;
            });

        if (userIt != usersData.end()) {
            usersData.erase(userIt);
            ofstream usersOutFile("users.json");
            if (usersOutFile.is_open()) {
                usersOutFile << usersData.dump(4);
                usersOutFile.close();
            }
            else {
                wxMessageBox("Cannot save to users.json file.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }
        else {
            wxMessageBox("User not found in users.json.", "Error", wxOK | wxICON_ERROR);
        }

        wxMessageBox("Student and their grades removed successfully.", "Success", wxOK | wxICON_INFORMATION);

        //Odœwie¿enie listy studentów
        RefreshStudentList(); 

		//Zamkniêcie okna dialogowego
        dlg.EndModal(wxID_OK);
        }, wxID_OK);

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    dlg.ShowModal();
}



//Event do dodawania studenta

void AdminPanel::OnAddStudent(wxCommandEvent& event)
{
    ShowAddUserDialog("Student");
    
}


//Event do dodawania nauczyciela

void AdminPanel::OnAddTeacher(wxCommandEvent& event)
{
    ShowAddUserDialog("Teacher");
    
}




//Dodanie studenta lub nauczyciela (na podstawie roli)

void AdminPanel::ShowAddUserDialog(const wxString& role)
{
    
    wxDialog dlg(this, wxID_ANY, "Add New " + role, wxDefaultPosition, wxSize(400, 600));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	//Nag³ówek
    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Add New " + role);
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 10);

    //Tworzenie pól wejœciowych
    auto createField = [&](const wxString& labelText) {
        wxBoxSizer* fieldSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label = new wxStaticText(panel, wxID_ANY, labelText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        fieldSizer->Add(label, 0, wxALL | wxALIGN_CENTER, 5);

        wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
        fieldSizer->Add(textCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

        return std::make_pair(fieldSizer, textCtrl);
        };


	//Dodanie odpowiednich pól w zale¿noœci od roli
    if (role == "Student") {

		//Pola dla studenta
        auto firstNameField = createField("First Name:");
        auto lastNameField = createField("Last Name:");
        auto emailField = createField("Email:");
        auto majorField = createField("Major:");
        auto yearField = createField("Year:");

        sizer->Add(firstNameField.first, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(lastNameField.first, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(emailField.first, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(majorField.first, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(yearField.first, 0, wxALL | wxALIGN_CENTER, 5);

        wxButton* addButton = new wxButton(panel, wxID_ANY, "Add Student");
        sizer->Add(addButton, 0, wxALL | wxALIGN_CENTER, 10);

        addButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
            //Pobranie danych z pól
            wxString firstName = firstNameField.second->GetValue();
            wxString lastName = lastNameField.second->GetValue();
            wxString email = emailField.second->GetValue();
            wxString major = majorField.second->GetValue();
            wxString yearStr = yearField.second->GetValue();

			//Walidacja pól
            if (firstName.IsEmpty() || lastName.IsEmpty() || email.IsEmpty() || major.IsEmpty() || yearStr.IsEmpty()) {
                wxMessageBox("All fields are required.", "Error", wxOK | wxICON_ERROR);
                return;
            }

            //Sprawdzenie poprawnoœci wprowadzonego roku
            int year = wxAtoi(yearStr);
            if (year <= 0) {
                wxMessageBox("Year must be a positive number.", "Error", wxOK | wxICON_ERROR);
                return;
            }

            //ZnajdŸ najwiêksze ID wœród istniej¹cych studentów
            vector<Students> students = Students::loadStudentsFromFile();
            int maxId = 0;

            for (const Students& student : students) {
                int currentId = std::stoi(student.id);
                if (currentId > maxId) {
                    maxId = currentId;
                }
            }

            //Tworzenie nowego studenta z ID o 1 wiêkszym od najwiêkszego
            int newId = maxId + 1;
            Students newStudent(std::to_string(newId),
                firstName.ToStdString(), lastName.ToStdString(), "",
                email.ToStdString(), major.ToStdString(), year, "", {});

            //Dodanie studenta
            try {
    
                //Dodanie studenta do pliku students.json
                Students::addStudent(newStudent, "userpassword");

                //Dodanie nowego u¿ytkownika do danych
                students.push_back(newStudent);

                //Zapisanie listy studentów do pliku
                Students::saveStudentsToFile(students);
              

                wxMessageBox("Student added successfully.", "Success", wxOK | wxICON_INFORMATION);

                //Zamkniêcie okna dialogowego
                dlg.EndModal(wxID_OK);

                //odœwie¿enie listy studentów
                RefreshStudentList();
            }
            catch (const std::exception& e) {
                wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
            }
            });

        panel->SetSizer(sizer);
        dlg.ShowModal();
    }

	//Dodanie pól dla nauczyciela
    else if(role == "Teacher") {

		//Pola dla nauczyciela
        auto firstNameField = createField("First Name:");
        auto lastNameField = createField("Last Name:");
        auto emailField = createField("Email:");
        auto subjectField = createField("Subject:");
        
        sizer->Add(firstNameField.first, 0, wxALL | wxEXPAND, 5);
        sizer->Add(lastNameField.first, 0, wxALL | wxEXPAND, 5);
        sizer->Add(emailField.first, 0, wxALL | wxEXPAND, 5);
        sizer->Add(subjectField.first, 0, wxALL | wxEXPAND, 5);
        

        //Przyciski
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* addBtn = new wxButton(panel, wxID_OK, "Add");
        wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
        buttonSizer->Add(addBtn, 0, wxALL, 5);
        buttonSizer->Add(cancelBtn, 0, wxALL, 5);

        sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        panel->SetSizer(sizer);

		//Pobranie danych z pól
        dlg.Bind(wxEVT_BUTTON, [&dlg, &firstNameField, &lastNameField, &emailField, &subjectField, this](wxCommandEvent&) {
            wxString firstName = firstNameField.second->GetValue();
            wxString lastName = lastNameField.second->GetValue();
            wxString email = emailField.second->GetValue();
            wxString major = subjectField.second->GetValue();
          

            //Walidacja pól
            if (firstName.IsEmpty() || lastName.IsEmpty() || email.IsEmpty() || major.IsEmpty()) {
                wxMessageBox("Please fill in all fields.", "Error", wxOK | wxICON_ERROR);
                return;
            }

            // Wczytaj istniej¹cych studentów z pliku
            vector<Teacher> Teachers = Teacher::loadTeachersFromFile();

            // Generowanie unikalnego ID dla nowego studenta
            const std::string newId = std::to_string(Teachers.size() + 1);

            //Tworzenie nowego obiektu studenta
            Teacher newTeacher(newId, firstName.ToStdString(), lastName.ToStdString(), major.ToStdString(), email.ToStdString(), vector<string>(), {});
            //Dodanie nowego studenta do pliku i bazy u¿ytkowników
            Teacher::addTeacher(newTeacher, "userpassword");

            //Dodanie nowego studenta do listy
            Teachers.push_back(newTeacher);

            //Zapisanie listy studentów do pliku
            Teacher::saveTeachersToFile(Teachers);

			// Odœwie¿enie listy nauczycieli
            RefreshStudentList();

            wxMessageBox("Teacher added successfully!", "Success", wxOK | wxICON_INFORMATION);
            dlg.EndModal(wxID_OK);
            }, wxID_OK);

        dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

        dlg.ShowModal();
    }
   
}





//Usuniêcie nauczyciela wraz z jego egzaminami i ocenami

void AdminPanel::OnRemoveTeacher(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Remove Teacher", wxDefaultPosition, wxSize(300, 600));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	//Nag³ówek
    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Remove Teacher");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 10);

	//Wczytanie nauczycieli z pliku teachers.json
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();

    //Tworzenie listy rozwijalnej z nauczycielami
    wxStaticText* teacherLabel = new wxStaticText(panel, wxID_ANY, "Select Teacher:");
    sizer->Add(teacherLabel, 0, wxALL | wxALIGN_CENTER, 5);

    wxChoice* teacherChoice = new wxChoice(panel, wxID_ANY);
    for (const Teacher& teacher : teachers) {
        teacherChoice->Append(teacher.first_name + " " + teacher.last_name);
    }
    sizer->Add(teacherChoice, 0, wxALL | wxEXPAND, 5);

    //Przyciski
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* removeBtn = new wxButton(panel, wxID_OK, "Remove");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
    buttonSizer->Add(removeBtn, 0, wxALL, 5);
    buttonSizer->Add(cancelBtn, 0, wxALL, 5);

    sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&dlg, teacherChoice, &teachers, this](wxCommandEvent&) {
        int selectedIndex = teacherChoice->GetSelection();

        if (selectedIndex == wxNOT_FOUND) {
            wxMessageBox("Please select a teacher.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        //Pobranie wybranego nauczyciela
        const Teacher& selectedTeacher = teachers[selectedIndex];
        string teacherId = selectedTeacher.id;
        string teacherEmail = selectedTeacher.email;
        vector<string> examsToRemove = selectedTeacher.exams;
        vector<string> gradesToRemove = selectedTeacher.grades;

        //Usuniêcie nauczyciela z listy
        teachers.erase(teachers.begin() + selectedIndex);
        Teacher::saveTeachersToFile(teachers);

        //Wczytanie i aktualizacja listy egzaminów
        vector<Exam> exams = Exam::loadExamsFromFile();
        exams.erase(remove_if(exams.begin(), exams.end(), [&examsToRemove](const Exam& exam) {
            return find(examsToRemove.begin(), examsToRemove.end(), exam.id) != examsToRemove.end();
            }), exams.end());
        Exam::saveExamsToFile(exams);


        //Wczytanie i aktualizacja listy ocen
        vector<Grade> grades = Grade::loadGradesFromFile();
        grades.erase(remove_if(grades.begin(), grades.end(), [&gradesToRemove](const Grade& grade) {
            return find(gradesToRemove.begin(), gradesToRemove.end(), grade.id) != gradesToRemove.end();
            }), grades.end());
        Grade::saveGradesToFile(grades);


        //Wczytanie i aktualizacja studentów
        vector<Students> students = Students::loadStudentsFromFile();
        for (Students& student : students) {
            student.grades.erase(remove_if(student.grades.begin(), student.grades.end(), [&gradesToRemove](const string& gradeId) {
                return find(gradesToRemove.begin(), gradesToRemove.end(), gradeId) != gradesToRemove.end();
                }), student.grades.end());
        }
        Students::saveStudentsToFile(students);


        //Usuniêcie u¿ytkownika z pliku `users.json`
        ifstream usersFile("users.json");
        json usersData;

        if (usersFile.is_open()) {
            usersFile >> usersData;
            usersFile.close();
        }
        else {
            wxMessageBox("Cannot open users.json file.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        auto userIt = find_if(usersData.begin(), usersData.end(), [&teacherEmail](const json& user) {
            return user["login"] == teacherEmail;
            });

        if (userIt != usersData.end()) {
            usersData.erase(userIt);
            ofstream usersOutFile("users.json");
            if (usersOutFile.is_open()) {
                usersOutFile << usersData.dump(4);
                usersOutFile.close();
            }
            else {
                wxMessageBox("Cannot save to users.json file.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }
        else {
            wxMessageBox("User not found in users.json.", "Error", wxOK | wxICON_ERROR);
        }

        wxMessageBox("Teacher and their exams and grades removed successfully.", "Success", wxOK | wxICON_INFORMATION);
        
        //Odœwie¿enie listy nauczycieli
        RefreshTeacherList();

        dlg.EndModal(wxID_OK);
        }, wxID_OK);

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    dlg.ShowModal();

}



