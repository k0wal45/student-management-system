//Pliki nag��wkowe
#include "TeacherPanel.h"
#include "MainFrame.h"
#include "Teacher.h"
#include "Students.h"
#include "Grade.h"
//WxWidgets
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
//Biblioteki 
#include <set>
#include <regex>


//Konstruktor

TeacherPanel::TeacherPanel(wxWindow* parent, const wxString& teacherEmail)
    : wxPanel(parent), teacherEmail(teacherEmail), notebook(nullptr)
{
    SetBackgroundColour(wxColour(240, 240, 240));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSize(800, 600);


    //Wczytaj dane nauczycieli
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();

    //Znajd� nauczyciela na podstawie adresu e-mail
    wxString emailToFind = teacherEmail; // Przypisz warto�� do zmiennej lokalnej
    auto it = std::find_if(teachers.begin(), teachers.end(), [&emailToFind](const Teacher& teacher) {
        return teacher.email == emailToFind.ToStdString();
    });


    wxString headerText;
    if (it != teachers.end()) {

        //Je�li nauczyciel zosta� znaleziony, ustaw imi� i nazwisko w nag��wku
        const Teacher& teacher = *it;
        headerText = "Teacher Panel: " + wxString::FromUTF8(teacher.first_name + " " + teacher.last_name);
    }

    else {
        //Je�li nie znaleziono nauczyciela, przekieruj do panelu logowania
        wxMessageBox("Teacher not found: " + teacherEmail, "Error", wxOK | wxICON_ERROR);
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        if (frame) {
            frame->ShowLoginPanel();
        }
        //Zako�cz konstruktor, aby nie inicjalizowa� panelu
        return; 
    }

    //Nag��wek
    wxStaticText* title = new wxStaticText(this, wxID_ANY, headerText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(title, 0, wxALL | wxEXPAND, 10);

    //Panel wyszukiwania
    wxPanel* searchPanel = new wxPanel(this);
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);

    searchCtrl = new wxTextCtrl(searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    searchCtrl->SetHint("Wpisz imi� ucznia...");
    wxButton* searchBtn = new wxButton(searchPanel, wxID_ANY, "Szukaj");

    searchSizer->Add(searchCtrl, 0, wxALL, 5);
    searchSizer->Add(searchBtn, 0, wxALL, 5);
    searchPanel->SetSizer(searchSizer);

    //Notebook z zak�adkami
    notebook = new wxNotebook(this, wxID_ANY);

    //Zak�adka Studenci
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);

    //Dodanie kolumn
    studentsList = new wxListCtrl(studentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    studentsList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    studentsList->AppendColumn("First Name", wxLIST_FORMAT_LEFT, 100);
    studentsList->AppendColumn("Last Name", wxLIST_FORMAT_LEFT, 100);
    studentsList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);
    studentsList->AppendColumn("Major", wxLIST_FORMAT_LEFT, 150);
    studentsList->AppendColumn("Year", wxLIST_FORMAT_LEFT, 50);


    //Wczytanie i wy�wietlenie student�w z pliku JSON
    vector<Students> students = Students::loadStudentsFromFile();
    for (const Students& student : students) {
        long index = studentsList->InsertItem(studentsList->GetItemCount(), student.id);
        studentsList->SetItem(index, 1, student.first_name);
        studentsList->SetItem(index, 2, student.last_name);
        studentsList->SetItem(index, 3, student.email);
        studentsList->SetItem(index, 4, student.major);
        studentsList->SetItem(index, 5, wxString::Format("%d", student.year));
    }

    //Dodanie stylu
    studentsList->SetBackgroundColour(wxColour(255, 255, 255));
    studentsList->SetTextColour(wxColour(0, 0, 0));
    studentsList->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    studentsSizer->Add(studentsList, 1, wxALL | wxEXPAND, 5);
    studentsPanel->SetSizer(studentsSizer);

    //Zak�adka Oceny
    wxPanel* gradesPanel = new wxPanel(notebook);
    wxBoxSizer* gradesSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addGradeBtn = new wxButton(gradesPanel, wxID_ANY, "Add Grade");
    addGradeBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnAddGrade, this);

    wxButton* removeGradeBtn = new wxButton(gradesPanel, wxID_ANY, "Remove Grade");
    removeGradeBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnRemoveGrade, this);

    //Sizer dla przycisk�w
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL); 
    buttonSizer->Add(addGradeBtn, 0, wxALL, 5);
    buttonSizer->Add(removeGradeBtn, 0, wxALL, 5);

	//Lista ocen
    gradesList = new wxListCtrl(gradesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    gradesList->AppendColumn("Student", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Grade", wxLIST_FORMAT_LEFT, 80);
    gradesList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    gradesList->AppendColumn("Comment", wxLIST_FORMAT_LEFT, 200);
   

    //Wczytanie i wy�wietlenie ocen z pliku JSON
    vector<Grade> allGrades = Grade::loadGradesFromFile();

	//Wczytaj list� student�w z pliku JSON
    vector<Students> allStudents = Students::loadStudentsFromFile();

    //Pobierz list� ocen aktualnie zalogowanego nauczyciela
    vector<string> teacherGrades = it->grades; 

    for (const Grade& grade : allGrades) {
        //Sprawd�, czy ocena nale�y do aktualnie zalogowanego nauczyciela
        if (std::find(teacherGrades.begin(), teacherGrades.end(), grade.id) != teacherGrades.end()) {

            //Znajd� studenta, kt�ry ma t� ocen�
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
    }
    //Dodanie sizeru przycisk�w do g��wnego sizeru
    gradesSizer->Add(buttonSizer, 0, wxALIGN_LEFT | wxALL, 5); 
    gradesSizer->Add(gradesList, 1, wxALL | wxEXPAND, 5);
    gradesPanel->SetSizer(gradesSizer);

    //Zak�adka Sprawdziany
    wxPanel* examsPanel = new wxPanel(notebook);
    wxBoxSizer* examsSizer = new wxBoxSizer(wxVERTICAL);

    //Przyciski
    wxButton* addExamBtn = new wxButton(examsPanel, wxID_ANY, "Add Exam");
    addExamBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnAddExam, this);
    wxButton* removeExamBtn = new wxButton(examsPanel, wxID_ANY, "Remove Exam");
    removeExamBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnRemoveExam, this);

    //Kolumny
    examsList = new wxListCtrl(examsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    examsList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    examsList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);
    examsList->AppendColumn("Description", wxLIST_FORMAT_LEFT, 250);

	//Wy�wietlenie egzamin�w
    RefreshExamsList();

    examsSizer->Add(addExamBtn, 0, wxALL, 5);
    examsSizer->Add(removeExamBtn, 0, wxALL, 5);
    examsSizer->Add(examsList, 1, wxALL | wxEXPAND, 5);
    examsPanel->SetSizer(examsSizer);

	//Dodanie zak�adek do notebooka
    notebook->AddPage(studentsPanel, "Students");
    notebook->AddPage(gradesPanel, "Grades");
    notebook->AddPage(examsPanel, "Exams");

    mainSizer->Add(searchPanel, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(notebook, 1, wxALL | wxEXPAND, 5);

    //Eventy
    searchBtn->Bind(wxEVT_BUTTON, &TeacherPanel::OnSearch, this);

    // Wyszukiwanie podczas wpisywania
    searchCtrl->Bind(wxEVT_TEXT, &TeacherPanel::OnSearch, this); 

    //Przycisk powrotu
    wxButton* backBtn = new wxButton(this, wxID_ANY, "Back to Login");
    backBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
        frame->ShowLoginPanel();
        });

    mainSizer->Add(backBtn, 0, wxALL | wxALIGN_RIGHT, 5);
    SetSizer(mainSizer);
}





//Usuni�cie egzaminu

void TeacherPanel::OnRemoveExam(wxCommandEvent& event)
{
    long selectedItem = examsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedItem == -1) {
        wxMessageBox("Please select an exam to remove.", "Error", wxOK | wxICON_ERROR);
        return;
    }

	//Pobierz dane wybranego egzaminu
    wxString subject = examsList->GetItemText(selectedItem, 0);
    wxString date = examsList->GetItemText(selectedItem, 1);


    // Wczytaj wszystkie egzaminy
    vector<Exam> allExams = Exam::loadExamsFromFile();


    // Znajd� egzamin do usuni�cia
    auto it = std::find_if(allExams.begin(), allExams.end(), [&](const Exam& exam) {
        return wxString::FromUTF8(exam.subject) == subject && wxString::FromUTF8(exam.date) == date;
        });

    if (it != allExams.end()) {

        // Usu� egzamin z listy egzamin�w nauczyciela
        vector<Teacher> allTeachers = Teacher::loadTeachersFromFile();
        auto teacherIt = std::find_if(allTeachers.begin(), allTeachers.end(), [&](const Teacher& teacher) {
            return teacher.email == teacherEmail.ToStdString();
            });

        if (teacherIt != allTeachers.end()) {
            Teacher& teacher = *teacherIt;
            auto examIdIt = std::find(teacher.exams.begin(), teacher.exams.end(), it->id);
            if (examIdIt != teacher.exams.end()) {
                teacher.exams.erase(examIdIt);
            }

            // Zapisz zaktualizowan� list� nauczycieli
            Teacher::saveTeachersToFile(allTeachers);
        }

        //Usu� egzamin z pliku
        Exam::removeExam(it->id);

		//Od�wie� list� egzamin�w
        RefreshExamsList();

        wxMessageBox("Exam removed successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Exam not found.", "Error", wxOK | wxICON_ERROR);
    }
}




//Wygenerowanie nowego ID dla oceny

std::string GenerateUniqueGradeId(const std::vector<Grade>& allGrades) {
    int maxId = 0;

    //Wyszukaj najwi�ksze ID w ocenach
    std::regex idRegex("^g(\\d+)$");
    for (const auto& grade : allGrades) {
        std::smatch match;
        if (std::regex_match(grade.id, match, idRegex)) {
            int currentId = std::stoi(match[1].str());
            maxId = std::max(maxId, currentId);
        }
    }

    //Zwr�� nowe ID w formacie "g" + (maxId + 1)
    return "g" + std::to_string(maxId + 1);
}


//Wygenerowanie nowego ID dla egzaminu

std::string GenerateUniqueExamId(const std::vector<Exam>& allExams) {
    int maxId = 0;

    //Wyszukaj najwi�ksze ID w egzaminach
    std::regex idRegex("^e(\\d+)$");
    for (const auto& exam : allExams) {
        std::smatch match;
        if (std::regex_match(exam.id, match, idRegex)) {
            int currentId = std::stoi(match[1].str());
            maxId = std::max(maxId, currentId);
        }
    }

    //Zwr�� nowe ID w formacie "e" + (maxId + 1)
    return "e" + std::to_string(maxId + 1);
}


//Wyszukiwanie studenta (event)

void TeacherPanel::OnSearch(wxCommandEvent& event)
{
    //Pobierz warto�� z pola
    wxString searchText = searchCtrl->GetValue();
    int activeTab = notebook->GetSelection();

    if (activeTab == 0) { // Zak�adka Studenci
        FilterData(searchText, false);
    }
    else if (activeTab == 1) { // Zak�adka Oceny
        FilterData(searchText, true);
    }
}



//Filtrowanie danych na podstawie wyszukiwania

void TeacherPanel::FilterData(const wxString& searchText, bool filterGrades)
{
    wxBusyCursor busyCursor;


    //Filetorwanie w zak�adce oceny
    if (filterGrades) {
        if (!gradesList) return;

        gradesList->Freeze();
        gradesList->DeleteAllItems();

        try {
			//Wczytaj wszystkie oceny i student�w z plik�w
            vector<Grade> allGrades = Grade::loadGradesFromFile();
            vector<Students> allStudents = Students::loadStudentsFromFile();

			//Pobierz warto�� z pola wyszukiwania i przekszta�� na ma�e litery
            wxString searchLower = searchText.Lower();

			//Sprawd�, kt�re dane pasuj� do wyszukiwania
            for (const Grade& grade : allGrades) {
                for (const Students& student : allStudents) {
                    if (std::find(student.grades.begin(), student.grades.end(), grade.id) != student.grades.end()) {
                        wxString studentName = student.first_name + " " + student.last_name;
                        wxString subject = grade.subject;
                        wxString comment = grade.comment;

                        bool match = searchText.IsEmpty() || studentName.Lower().Contains(searchLower) ||
                        subject.Lower().Contains(searchLower) || comment.Lower().Contains(searchLower);

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
        //Filetorwanie w zak�adce studenci
        if (!studentsList) return;

        studentsList->Freeze();
        studentsList->DeleteAllItems();

        try {
			//Wczytaj wszystkich student�w z pliku
            vector<Students> allStudents = Students::loadStudentsFromFile();

            //Pobierz warto�� z pola wyszukiwania i przekszta�� na ma�e litery
            wxString searchLower = searchText.Lower();

			//Sprawd�, kt�re dane pasuj� do wyszukiwania
            for (const Students& student : allStudents) {
                wxString name = student.first_name;
                wxString surname = student.last_name;

                bool match = searchText.IsEmpty() ||name.Lower().Contains(searchLower) ||surname.Lower().Contains(searchLower);

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




//Dodanie oceny (event)

void TeacherPanel::OnAddGrade(wxCommandEvent& event)
{
    //Wy�wietlenie dialogu do dodania oceny
    ShowAddGradeDialog();

    //Od�wie�enie listy ocen
    RefreshGradesList();
}



// Od�wie�enie listy egzamin�w

void TeacherPanel::RefreshExamsList() {
    // Wczytaj list� nauczycieli z pliku
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();

    //Znajd� zalogowanego nauczyciela na podstawie adresu email
    auto it = std::find_if(teachers.begin(), teachers.end(), [this](const Teacher& teacher) {
        return teacher.email == teacherEmail.ToStdString();
        });

    if (it == teachers.end()) {
        wxMessageBox("Logged-in teacher not found.", "Error", wxOK | wxICON_ERROR);
        return;
    }


    const Teacher& loggedTeacher = *it;

    //Wczytaj list� egzamin�w z pliku
    vector<Exam> allExams = Exam::loadExamsFromFile();

    // Wyczy�� list� egzamin�w
    examsList->Freeze();
    examsList->DeleteAllItems();

    //Wy�wietl egzaminy utworzone przez zalogowanego nauczyciela
    for (const string& examId : loggedTeacher.exams) {
        auto examIt = std::find_if(allExams.begin(), allExams.end(), [&examId](const Exam& exam) {
            return exam.id == examId;
            });

        if (examIt != allExams.end()) {
            const Exam& exam = *examIt;

            long index = examsList->InsertItem(examsList->GetItemCount(), exam.subject);
            examsList->SetItem(index, 1, exam.date);
            examsList->SetItem(index, 2, exam.comment);
        }
    }

    examsList->Thaw();
}


//Usuni�cie oceny (event)

void TeacherPanel::OnRemoveGrade(wxCommandEvent& event)
{
    //Usuni�cie oceny studenta
    ShowRemoveGradeDialog();
}




//Dodanie egzaminu (event)

void TeacherPanel::OnAddExam(wxCommandEvent& event)
{
	//Dodanie egzaminu
    ShowAddExamDialog();

	//Od�wie�enie listy egzamin�w
    RefreshExamsList();
}



//Usuni�cie oceny studenta

void TeacherPanel::ShowRemoveGradeDialog() {

    wxDialog* removeGradeDialog = new wxDialog(this, wxID_ANY, "Remove Grade", wxDefaultPosition, wxSize(1000, 700));
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

    // Pole wyboru studenta
    wxStaticText* studentLabel = new wxStaticText(removeGradeDialog, wxID_ANY, "Student:");
    wxComboBox* studentComboBox = new wxComboBox(removeGradeDialog, wxID_ANY);

	//Wczytaj wszystkich student�w z pliku
    vector<Students> allStudents = Students::loadStudentsFromFile();

	//Dodaj student�w do pola wyboru
    for (const Students& student : allStudents) {
        studentComboBox->Append(student.first_name + " " + student.last_name);
    }

    //Pole wyboru przedmiotu
    wxStaticText* subjectLabel = new wxStaticText(removeGradeDialog, wxID_ANY, "Subject:");
    wxTextCtrl* subjectTextCtrl = new wxTextCtrl(removeGradeDialog, wxID_ANY);

    //Pole oceny
    wxStaticText* gradeLabel = new wxStaticText(removeGradeDialog, wxID_ANY, "Grade:");
    wxTextCtrl* gradeTextCtrl = new wxTextCtrl(removeGradeDialog, wxID_ANY);

    //Przyciski
    wxButton* removeButton = new wxButton(removeGradeDialog, wxID_OK, "Remove");
    wxButton* cancelButton = new wxButton(removeGradeDialog, wxID_CANCEL, "Cancel");

    //Uk�ad element�w
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
        //Pobierz dane z p�l
        wxString studentName = studentComboBox->GetValue();
        wxString subject = subjectTextCtrl->GetValue();
        double grade = wxAtof(gradeTextCtrl->GetValue());

        //Usu� ocen� od odpowiedniego studenta
        for (Students& student : allStudents) {
            if (studentName == student.first_name + " " + student.last_name) {

				//Sprawd�, czy student ma ocen� z danego przedmiotu
                vector<string>& studentGrades = student.grades;
                vector<Grade> allGrades = Grade::loadGradesFromFile();

                auto it = std::find_if(allGrades.begin(), allGrades.end(), [&](const Grade& g) {
                    return g.subject == subject.ToStdString() && g.grade == grade;
                    });

                if (it != allGrades.end()) {

                    //Usu� ocen� od studenta
                    student.removeGrade(it->id);

                    // Usu� ocen� od nauczyciela
                    vector<Teacher> allTeachers = Teacher::loadTeachersFromFile();
                    auto teacherIt = std::find_if(allTeachers.begin(), allTeachers.end(), [&](const Teacher& teacher) {
                        return teacher.email == teacherEmail.ToStdString();
                        });

                    if (teacherIt != allTeachers.end()) {
                        Teacher& teacher = *teacherIt;
                        auto gradeIdIt = std::find(teacher.grades.begin(), teacher.grades.end(), it->id);
                        if (gradeIdIt != teacher.grades.end()) {
                            teacher.grades.erase(gradeIdIt);
                        }

                        // Zapisz zaktualizowan� list� nauczycieli
                        Teacher::saveTeachersToFile(allTeachers);
                    }

                    // Usu� ocen� z listy ocen
                    allGrades.erase(it);

                    //Zapisz zaktualizowan� list� ocen
                    Grade::saveGradesToFile(allGrades);

                    //Zapisz zaktualizowan� list� student�w
                    Students::saveStudentsToFile(allStudents);

                    //Od�wie� list� ocen
                    RefreshGradesList();
                    wxMessageBox("Grade removed successfully!", "Success", wxOK | wxICON_INFORMATION);
                }
                else {
                    wxMessageBox("Grade not found!", "Error", wxOK | wxICON_ERROR);
                }
                break;
            }
        }
    }

    removeGradeDialog->Destroy();
}






//Dodawanie oceny

void TeacherPanel::ShowAddGradeDialog()
{
    wxDialog* addGradeDialog = new wxDialog(this, wxID_ANY, "Add Grade", wxDefaultPosition, wxSize(1000, 700));
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

    //Pole wyboru studenta
    wxStaticText* studentLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Student:");
    wxComboBox* studentComboBox = new wxComboBox(addGradeDialog, wxID_ANY);
    vector<Students> allStudents = Students::loadStudentsFromFile();

	//Dodaj student�w do pola wyboru
    for (const Students& student : allStudents) {
        studentComboBox->Append(student.first_name + " " + student.last_name);
    }

    //Pole wyboru przedmiotu
    wxStaticText* subjectLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Subject:");
    wxComboBox* subjectComboBox = new wxComboBox(addGradeDialog, wxID_ANY);

	//Wczytaj wszystkie oceny z pliku
    vector<Grade> allGrades = Grade::loadGradesFromFile();

    //Przedmioty
    set<string> subjects;
    for (const Grade& grade : allGrades) {
        subjects.insert(grade.subject);
    }
    for (const string& subject : subjects) {
        subjectComboBox->Append(subject);
    }

    //Wyb�r oceny
    wxStaticText* gradeLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Grade:");
    wxComboBox* gradeComboBox = new wxComboBox(addGradeDialog, wxID_ANY);
    set<double> grades;
    for (const Grade& grade : allGrades) {
        grades.insert(grade.grade);
    }
    for (double gradeValue : grades) {
        gradeComboBox->Append(wxString::Format("%.1f", gradeValue));
    }

    //Pole komentarza
    wxStaticText* commentLabel = new wxStaticText(addGradeDialog, wxID_ANY, "Comment:");
    wxTextCtrl* commentTextCtrl = new wxTextCtrl(addGradeDialog, wxID_ANY);

    // Przyciski
    wxButton* saveButton = new wxButton(addGradeDialog, wxID_OK, "Save");
    wxButton* cancelButton = new wxButton(addGradeDialog, wxID_CANCEL, "Cancel");

    
    //Uk�ad element�w
    wxBoxSizer* studentSizer = new wxBoxSizer(wxVERTICAL);
    studentSizer->Add(studentLabel, 0, wxCENTER, 10); // Zwi�kszenie marginesu
    studentSizer->Add(studentComboBox, 1, wxEXPAND);

    wxBoxSizer* subjectSizer = new wxBoxSizer(wxVERTICAL);
    subjectSizer->Add(subjectLabel, 0, wxCENTER, 10); // Zwi�kszenie marginesu
    subjectSizer->Add(subjectComboBox, 1, wxEXPAND);

    wxBoxSizer* gradeSizer = new wxBoxSizer(wxVERTICAL);
    gradeSizer->Add(gradeLabel, 0, wxCENTER, 10); // Zwi�kszenie marginesu
    gradeSizer->Add(gradeComboBox, 1, wxEXPAND);

    wxBoxSizer* commentSizer = new wxBoxSizer(wxVERTICAL);
    commentSizer->Add(commentLabel, 0, wxCENTER, 10); // Zwi�kszenie marginesu
    commentSizer->Add(commentTextCtrl, 1, wxEXPAND);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(saveButton, 0, wxRIGHT, 10); // Zwi�kszenie marginesu
    buttonSizer->Add(cancelButton, 0, wxLEFT, 10);

    dialogSizer->Add(studentSizer, 0, wxALL | wxEXPAND, 10); // Zwi�kszenie margines�w
    dialogSizer->Add(subjectSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(gradeSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(commentSizer, 0, wxALL | wxEXPAND, 10);
    dialogSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    addGradeDialog->SetSizer(dialogSizer);
    dialogSizer->Fit(addGradeDialog);

	//Obs�uga zdarzenia przycisku "Zapisz"

    if (addGradeDialog->ShowModal() == wxID_OK) {
        //Pobierz dane z p�l
        wxString studentName = studentComboBox->GetValue();
        wxString subject = subjectComboBox->GetValue();
        double grade = wxAtof(gradeComboBox->GetValue());
        wxString comment = commentTextCtrl->GetValue();

        //Wczytaj list� nauczycieli z pliku
        vector<Teacher> teachers = Teacher::loadTeachersFromFile();

        //Stw�rz nowe ID
        string newGradeId = GenerateUniqueGradeId(allGrades);

        //Dodaj ocen� do odpowiedniego studenta
        for (Students& student : allStudents) {
            if (studentName == student.first_name + " " + student.last_name) {
               
                Grade newGrade(newGradeId, grade, subject.ToStdString(), comment.ToStdString(), wxDateTime::Now().FormatISODate().ToStdString());
                student.addGrade(newGrade.id);

                student.grades.push_back(newGradeId);

                // Dodaj ocen� do listy wszystkich ocen
                allGrades.push_back(newGrade);

                // Zapisz zmiany do plik�w
                Grade::saveGradesToFile(allGrades);
                Students::saveStudentsToFile(allStudents);
                break;
            }
        }

		//Dodaj ocen� do listy ocen nauczyciela
        for (Teacher& teacher : teachers) {
            if (teacher.email == teacherEmail.ToStdString()) {
                // Dodaj ID oceny do listy ocen nauczyciela
                teacher.grades.push_back(newGradeId);

                // Zapisz zaktualizowan� list� nauczycieli do pliku
                Teacher::saveTeachersToFile(teachers);
                break;
            }
        }

        //Od�wie� list� ocen
        RefreshGradesList();
        wxMessageBox("Grade added successfully!", "Success", wxOK | wxICON_INFORMATION);
    }

    addGradeDialog->Destroy();
}





//Od�wie�enie listy ocen

void TeacherPanel::RefreshGradesList()
{

    vector<Teacher> teachers = Teacher::loadTeachersFromFile();

    //Znajd� zalogowanego nauczyciela na podstawie adresu e-mail
    for (const Teacher& teacher : teachers) {
        if (teacher.email == teacherEmail.ToStdString()) {
            // Wyczy�� list� ocen
            gradesList->Freeze();
            gradesList->DeleteAllItems();

            //Wczytaj wszystkie oceny z pliku
            vector<Grade> allGrades = Grade::loadGradesFromFile();
            vector<Students> allStudents = Students::loadStudentsFromFile();

            //Wy�wietl oceny wystawione przez zalogowanego nauczyciela
            for (const string& gradeId : teacher.grades) {
                auto it = std::find_if(allGrades.begin(), allGrades.end(), [&gradeId](const Grade& grade) {
                    return grade.id == gradeId;
                    });

                if (it != allGrades.end()) {
                    const Grade& grade = *it;

                    //Znajd� studenta, kt�ry ma t� ocen�
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
            }

            gradesList->Thaw();
            // Przerwij p�tl�, gdy znajdziesz zalogowanego nauczyciela
            break;
        }
    }
}





//Dodanie egzaminu

void TeacherPanel::ShowAddExamDialog()
{
    wxDialog dlg(this, wxID_ANY, "Add New Exam", wxDefaultPosition, wxSize(400, 300));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Add New Exam");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    //Lista rozwijalna dla przedmiot�w
    std::vector<std::string> subjects = { "Math", "Physics", "Chemistry", "Biology", "History" };
    wxArrayString wxSubjects;
    for (const auto& subject : subjects) {
        wxSubjects.Add(subject);
    }

	//Pole wyboru przedmiot�w
    wxChoice* subjectChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSubjects);

    //Pole dla daty
    wxDatePickerCtrl* dateCtrl = new wxDatePickerCtrl(panel, wxID_ANY);
	//Pole dla opisu
    wxTextCtrl* descriptionCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    wxFlexGridSizer* inputSizer = new wxFlexGridSizer(2, 5, 10);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Subject:"));
    inputSizer->Add(subjectChoice);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Date:"));
    inputSizer->Add(dateCtrl);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Description:"));
    inputSizer->Add(descriptionCtrl);

    //Przyciski
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
        // Pobierz dane z p�l
        wxString subject = subjectChoice->GetStringSelection();
        wxDateTime date = dateCtrl->GetValue();
        wxString description = descriptionCtrl->GetValue();
        
       // Wczytaj istniej�ce egzaminy z pliku
        std::vector<Exam> allExams = Exam::loadExamsFromFile();

        // Wygeneruj unikalne ID dla nowego egzaminu
        std::string newExamId = GenerateUniqueExamId(allExams);

        // Wczytaj list� nauczycieli
        std::vector<Teacher> allTeachers = Teacher::loadTeachersFromFile();

        // Znajd� aktualnie zalogowanego nauczyciela na podstawie adresu e-mail
        auto it = std::find_if(allTeachers.begin(), allTeachers.end(), [this](const Teacher& teacher) {
            return teacher.email == teacherEmail.ToStdString();
            });

        if (it == allTeachers.end()) {
            wxMessageBox("Logged-in teacher not found.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Pobierz ID aktualnie zalogowanego nauczyciela
        std::string teacherId = it->id;

        // Utw�rz nowy egzamin z poprawnym teacher_id (ID nauczyciela)
        Exam newExam(newExamId, date.FormatISODate().ToStdString(), description.ToStdString(), subject.ToStdString(), "t"+teacherId);

        // Dodaj egzamin do listy
        allExams.push_back(newExam);

        // Zapisz zaktualizowan� list� egzamin�w do pliku
        Exam::saveExamsToFile(allExams);

        // Dodaj ID egzaminu do listy egzamin�w nauczyciela
        it->exams.push_back(newExamId);

        // Zapisz zaktualizowan� list� nauczycieli do pliku
        Teacher::saveTeachersToFile(allTeachers);

        wxMessageBox("Exam added successfully!", "Success", wxOK | wxICON_INFORMATION);

    }
}


