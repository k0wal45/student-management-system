#include "AdminPanel.h"

#include <wx/listctrl.h>

#include "MainFrame.h"

#include <wx/datectrl.h>


#include "TeacherPanel.h"

#include <wx/datectrl.h>
#include "Students.h"
#include "Grade.h"
#include <set>
#include <regex>
#include <algorithm>
#include "StudentPanel.h"

#include <iterator>
#include <wx/arrstr.h>


AdminPanel::AdminPanel(wxWindow* parent)
    : wxPanel(parent)
{

    SetBackgroundColour(wxColour(240, 240, 240));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Nag³ówek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Administrator Panel", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    // Notebook z zak³adkami
    notebook = new wxNotebook(this, wxID_ANY);

    // Zak³adka Studenci
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);

    // Przyciski dodawania i usuwania studentów
    wxBoxSizer* studentButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addStudentBtn = new wxButton(studentsPanel, wxID_ANY, "Add New Student");
    addStudentBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnAddStudent, this);

    wxButton* removeStudentBtn = new wxButton(studentsPanel, wxID_ANY, "Remove Student");
    removeStudentBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnRemoveStudent, this);

    studentButtonSizer->Add(addStudentBtn, 0, wxALL, 5);
    studentButtonSizer->Add(removeStudentBtn, 0, wxALL, 5);

    studentsSizer->Add(studentButtonSizer, 0, wxALL, 5);

    // Lista studentów
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

    // Zak³adka Nauczyciele
    wxPanel* teachersPanel = new wxPanel(notebook);
    wxBoxSizer* teachersSizer = new wxBoxSizer(wxVERTICAL);

    // Przyciski dodawania nauczycieli
    wxBoxSizer* teacherButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addTeacherBtn = new wxButton(teachersPanel, wxID_ANY, "Add New Teacher");
    addTeacherBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnAddTeacher, this);

    teacherButtonSizer->Add(addTeacherBtn, 0, wxALL, 5);

    teachersSizer->Add(teacherButtonSizer, 0, wxALL, 5);

    // Lista nauczycieli
    wxListCtrl* teachersList = new wxListCtrl(teachersPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    teachersList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 80);
    teachersList->AppendColumn("First name", wxLIST_FORMAT_LEFT, 150);
    teachersList->AppendColumn("Last Name", wxLIST_FORMAT_LEFT, 150);
    teachersList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);
    teachersList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);

    teachersSizer->Add(teachersList, 1, wxALL | wxEXPAND, 5);
    teachersPanel->SetSizer(teachersSizer);

    notebook->AddPage(teachersPanel, "Teachers");

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




    void AdminPanel::RefreshStudentList()
    {

        if (!studentsList) return;

        studentsList->Freeze();
        studentsList->DeleteAllItems();

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






void AdminPanel::OnRemoveStudent(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Remove Student", wxDefaultPosition, wxSize(300, 600));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Remove Student");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 10);
    vector<Students> students = Students::loadStudentsFromFile();


    // Tworzenie list rozwijalnych
    wxStaticText* firstNameLabel = new wxStaticText(panel, wxID_ANY, "First Name:");
    sizer->Add(firstNameLabel, 0, wxALL | wxALIGN_CENTER, 5);

    wxChoice* firstNameChoice = new wxChoice(panel, wxID_ANY);
    for (const Students& student : students) {
        firstNameChoice->Append(student.first_name);
    }
    sizer->Add(firstNameChoice, 0, wxALL | wxEXPAND, 5);

    wxStaticText* lastNameLabel = new wxStaticText(panel, wxID_ANY, "Last Name:");
    sizer->Add(lastNameLabel, 0, wxALL | wxALIGN_CENTER, 5);

    wxChoice* lastNameChoice = new wxChoice(panel, wxID_ANY);
    for (const Students& student : students) {
        lastNameChoice->Append(student.last_name);
    }
    sizer->Add(lastNameChoice, 0, wxALL | wxEXPAND, 5);

    // Przyciski
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* removeBtn = new wxButton(panel, wxID_OK, "Remove");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
    buttonSizer->Add(removeBtn, 0, wxALL, 5);
    buttonSizer->Add(cancelBtn, 0, wxALL, 5);

    sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&dlg, firstNameChoice, lastNameChoice, &students, this](wxCommandEvent&) {
        int firstNameIndex = firstNameChoice->GetSelection();
        int lastNameIndex = lastNameChoice->GetSelection();

        if (firstNameIndex == wxNOT_FOUND || lastNameIndex == wxNOT_FOUND) {
            wxMessageBox("Please select both first name and last name.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        wxString selectedFirstName = firstNameChoice->GetString(firstNameIndex);
        wxString selectedLastName = lastNameChoice->GetString(lastNameIndex);

        // ZnajdŸ i usuñ studenta
        auto it = std::remove_if(students.begin(), students.end(), [&](const Students& student) {
            return student.first_name == selectedFirstName.ToStdString() && student.last_name == selectedLastName.ToStdString();
            });

        if (it != students.end()) {
            students.erase(it, students.end());
            Students::saveStudentsToFile(students);
            wxMessageBox("Student removed successfully.", "Success", wxOK | wxICON_INFORMATION);
            RefreshStudentList();
        }
        else {
            wxMessageBox("Student not found.", "Error", wxOK | wxICON_ERROR);
        }

        dlg.EndModal(wxID_OK);
        }, wxID_OK);

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    dlg.ShowModal();
}



//evant do dodawania studenta
void AdminPanel::OnAddStudent(wxCommandEvent& event)
{
    
    ShowAddUserDialog("Student");
    
}
//evant do dodawania nauczyciela
void AdminPanel::OnAddTeacher(wxCommandEvent& event)
{
    ShowAddUserDialog("Teacher");
    
}

void AdminPanel::ShowAddUserDialog(const wxString& role)
{
    wxDialog dlg(this, wxID_ANY, "Add New " + role, wxDefaultPosition, wxSize(400, 600));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Add New " + role);
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 10);

    // Tworzenie pól wejœciowych
    auto createField = [&](const wxString& labelText) {
        wxBoxSizer* fieldSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label = new wxStaticText(panel, wxID_ANY, labelText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        fieldSizer->Add(label, 0, wxALL | wxALIGN_CENTER, 5);

        wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
        fieldSizer->Add(textCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

        return std::make_pair(fieldSizer, textCtrl);
        };

    auto firstNameField = createField("First Name:");
    auto lastNameField = createField("Last Name:");
    auto emailField = createField("Email:");
    auto majorField = createField("Major:");
    auto yearField = createField("Year:");

    sizer->Add(firstNameField.first, 0, wxALL | wxEXPAND, 5);
    sizer->Add(lastNameField.first, 0, wxALL | wxEXPAND, 5);
    sizer->Add(emailField.first, 0, wxALL | wxEXPAND, 5);
    sizer->Add(majorField.first, 0, wxALL | wxEXPAND, 5);
    sizer->Add(yearField.first, 0, wxALL | wxEXPAND, 5);

    // Przyciski
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addBtn = new wxButton(panel, wxID_OK, "Add");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");
    buttonSizer->Add(addBtn, 0, wxALL, 5);
    buttonSizer->Add(cancelBtn, 0, wxALL, 5);

    sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&dlg, &firstNameField, &lastNameField, &emailField, &majorField, &yearField, this](wxCommandEvent&) {
        wxString firstName = firstNameField.second->GetValue();
        wxString lastName = lastNameField.second->GetValue();
        wxString email = emailField.second->GetValue();
        wxString major = majorField.second->GetValue();
        wxString yearStr = yearField.second->GetValue();

        if (firstName.IsEmpty() || lastName.IsEmpty() || email.IsEmpty() || major.IsEmpty() || yearStr.IsEmpty()) {
            wxMessageBox("Please fill in all fields.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        int year;
        if (!yearStr.ToLong((long*)&year) || year <= 0) {
            wxMessageBox("Year must be a positive number.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Wczytaj istniej¹cych studentów z pliku
        vector<Students> students = Students::loadStudentsFromFile();

        // Generowanie unikalnego ID dla nowego studenta
        std::string newId =  std::to_string(students.size() + 1);

        // Tworzenie nowego obiektu studenta
        Students newStudent(newId, firstName.ToStdString(), lastName.ToStdString(), "", email.ToStdString(), major.ToStdString(), year, "", {});

        // Dodanie nowego studenta do listy
        students.push_back(newStudent);

        // Zapisanie listy studentów do pliku
        Students::saveStudentsToFile(students);
        RefreshStudentList();
        wxMessageBox("Student added successfully!", "Success", wxOK | wxICON_INFORMATION);
       
        

        dlg.EndModal(wxID_OK);
        }, wxID_OK);

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);
    
    dlg.ShowModal();
}






