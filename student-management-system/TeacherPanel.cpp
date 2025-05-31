#include "TeacherPanel.h"
#include "MainFrame.h"
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
#include <wx/datectrl.h>
TeacherPanel::TeacherPanel(wxWindow* parent, const wxString& teacherName)
	: wxPanel(parent), teacherName(teacherName)
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
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    // Zak³adka Studenci
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);

    // Inicjalizacja listy studentów z dodatkowymi parametrami
    wxListCtrl* studentsList = new wxListCtrl(studentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    studentsList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 80);
    studentsList->AppendColumn("Name", wxLIST_FORMAT_LEFT, 150);
    studentsList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);


    // Przyk³adowi studenci
    long index = studentsList->InsertItem(0, "1");
    studentsList->SetItem(index, 1, "Alice Smith");
    studentsList->SetItem(index, 2, "alice@example.com");

    index = studentsList->InsertItem(1, "2");
    studentsList->SetItem(index, 1, "Bob Johnson");
    studentsList->SetItem(index, 2, "bob@example.com");
    
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

    wxListCtrl* gradesList = new wxListCtrl(gradesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    gradesList->AppendColumn("Student", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Subject", wxLIST_FORMAT_LEFT, 150);
    gradesList->AppendColumn("Grade", wxLIST_FORMAT_LEFT, 80);
    gradesList->AppendColumn("Date", wxLIST_FORMAT_LEFT, 120);

    // Przyk³adowe oceny
    index = gradesList->InsertItem(0, "Alice Smith");
    gradesList->SetItem(index, 1, "Mathematics");
    gradesList->SetItem(index, 2, "A");
    gradesList->SetItem(index, 3, "2023-10-15");

    index = gradesList->InsertItem(1, "Bob Johnson");
    gradesList->SetItem(index, 1, "Physics");
    gradesList->SetItem(index, 2, "B");
    gradesList->SetItem(index, 3, "2023-10-16");

    gradesSizer->Add(addGradeBtn, 0, wxALL, 5);
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
    index = examsList->InsertItem(0, "Mathematics");
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
    //zabezpieczenie przed nullptr
        if (studentsList && searchCtrl)
        {
            FilterStudents(searchCtrl->GetValue());
        }
    event.Skip();
}


void TeacherPanel::FilterStudents(const wxString& searchText)
{
    //jakiœ gówno kod tylko do testu

    if (!studentsList) return;

    wxBusyCursor busyCursor;
    studentsList->Freeze();

    try {
        wxString searchLower = searchText.Lower();

        for (int i = 0; i < studentsList->GetItemCount(); ++i) {
            wxString studentName = studentsList->GetItemText(i, 1).Lower();
            bool match = searchText.empty() || studentName.Contains(searchLower);
            studentsList->SetItemState(i, match ? 0 : wxLIST_STATE_DONTCARE, wxLIST_STATE_DONTCARE);
        }
    }
    catch (...) {
        wxLogError("Error during filtering");
    }

    studentsList->Thaw();
    studentsList->Refresh();
}

void TeacherPanel::OnAddGrade(wxCommandEvent& event)
{
    ShowAddGradeDialog();
}

void TeacherPanel::OnAddExam(wxCommandEvent& event)
{
    ShowAddExamDialog();
}

void TeacherPanel::ShowAddGradeDialog()
{
    wxDialog dlg(this, wxID_ANY, "Add New Grade", wxDefaultPosition, wxSize(500, 600));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Add New Grade");
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxChoice* studentChoice = new wxChoice(panel, wxID_ANY);
    studentChoice->Append("Alice Smith");
    studentChoice->Append("Bob Johnson");
    studentChoice->SetSelection(0);

    wxTextCtrl* subjectCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxChoice* gradeChoice = new wxChoice(panel, wxID_ANY);
    gradeChoice->Append("A");
    gradeChoice->Append("B");
    gradeChoice->Append("C");
    gradeChoice->Append("D");
    gradeChoice->Append("F");
    gradeChoice->SetSelection(0);

    wxDatePickerCtrl* dateCtrl = new wxDatePickerCtrl(panel, wxID_ANY);

    wxFlexGridSizer* inputSizer = new wxFlexGridSizer(2, 5, 10);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Student:"));
    inputSizer->Add(studentChoice);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Subject:"));
    inputSizer->Add(subjectCtrl);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Grade:"));
    inputSizer->Add(gradeChoice);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Date:"));
    inputSizer->Add(dateCtrl);

    wxButton* addBtn = new wxButton(panel, wxID_OK, "Add");
    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Cancel");

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(addBtn, 0, wxALL, 10);
    buttonSizer->Add(cancelBtn, 0, wxALL, 10);

    sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 20);
    sizer->Add(inputSizer, 0, wxALL | wxALIGN_CENTER, 20);
    sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 20);

    panel->SetSizer(sizer);

    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_OK); }, wxID_OK);
    dlg.Bind(wxEVT_BUTTON, [&dlg](wxCommandEvent&) { dlg.EndModal(wxID_CANCEL); }, wxID_CANCEL);

    if (dlg.ShowModal() == wxID_OK) {
        wxMessageBox("Grade added successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
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


