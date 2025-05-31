#include "AdminPanel.h"
#include "MainFrame.h"
#include <wx/listctrl.h>

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
    

    
    wxButton* addStudentBtn = new wxButton(studentsPanel, wxID_ANY, "Add New Student");
    addStudentBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnAddStudent, this);

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

    studentsSizer->Add(addStudentBtn, 0, wxALL, 5);
    studentsSizer->Add(studentsList, 1, wxALL | wxEXPAND, 5);
    studentsPanel->SetSizer(studentsSizer);

    // Zak³adka Nauczyciele
    wxPanel* teachersPanel = new wxPanel(notebook);
    wxBoxSizer* teachersSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addTeacherBtn = new wxButton(teachersPanel, wxID_ANY, "Add New Teacher");
    addTeacherBtn->Bind(wxEVT_BUTTON, &AdminPanel::OnAddTeacher, this);

    wxListCtrl* teachersList = new wxListCtrl(teachersPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    teachersList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 80);
    teachersList->AppendColumn("Name", wxLIST_FORMAT_LEFT, 150);
    teachersList->AppendColumn("Email", wxLIST_FORMAT_LEFT, 200);

    // Przyk³adowi nauczyciele
    index = teachersList->InsertItem(0, "1");
    teachersList->SetItem(index, 1, "John Doe");
    teachersList->SetItem(index, 2, "john@example.com");

    index = teachersList->InsertItem(1, "2");
    teachersList->SetItem(index, 1, "Jane Smith");
    teachersList->SetItem(index, 2, "jane@example.com");

    teachersSizer->Add(addTeacherBtn, 0, wxALL, 5);
    teachersSizer->Add(teachersList, 1, wxALL | wxEXPAND, 5);
    teachersPanel->SetSizer(teachersSizer);

    notebook->AddPage(studentsPanel, "Students");
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
    wxDialog dlg(this, wxID_ANY, "Add New " + role, wxDefaultPosition, wxSize(400, 300));

    wxPanel* panel = new wxPanel(&dlg);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Add New " + role);
    title->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxTextCtrl* firstNameCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxTextCtrl* lastNameCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxTextCtrl* emailCtrl = new wxTextCtrl(panel, wxID_ANY);

    wxFlexGridSizer* inputSizer = new wxFlexGridSizer(2, 5, 10);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "First Name:"));
    inputSizer->Add(firstNameCtrl);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Last Name:"));
    inputSizer->Add(lastNameCtrl);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Email:"));
    inputSizer->Add(emailCtrl);

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
        wxMessageBox(role + " added successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
}

void AdminPanel::OnEditUser(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Edit User", wxDefaultPosition, wxSize(500, 400));

    wxNotebook* notebook = new wxNotebook(&dlg, wxID_ANY);

    // Panel edycji studenta
    wxPanel* studentPanel = new wxPanel(notebook);
    wxFlexGridSizer* studentSizer = new wxFlexGridSizer(2, 5, 10);
    studentSizer->AddGrowableCol(1, 1);

    // Panel edycji nauczyciela
    wxPanel* teacherPanel = new wxPanel(notebook);
    wxFlexGridSizer* teacherSizer = new wxFlexGridSizer(2, 5, 10);
    teacherSizer->AddGrowableCol(1, 1);

    // Wspólne pola
    wxTextCtrl* firstNameCtrl = new wxTextCtrl(studentPanel, wxID_ANY, "John");
    wxTextCtrl* lastNameCtrl = new wxTextCtrl(studentPanel, wxID_ANY, "Doe");
    wxTextCtrl* emailCtrl = new wxTextCtrl(studentPanel, wxID_ANY, "john.doe@example.com");

    // Pola specyficzne dla studenta
    wxTextCtrl* studentIdCtrl = new wxTextCtrl(studentPanel, wxID_ANY, "12345");
    wxTextCtrl* groupCtrl = new wxTextCtrl(studentPanel, wxID_ANY, "CS-101");

    // Pola specyficzne dla nauczyciela
    wxTextCtrl* teacherIdCtrl = new wxTextCtrl(teacherPanel, wxID_ANY, "T-456");
    wxTextCtrl* departmentCtrl = new wxTextCtrl(teacherPanel, wxID_ANY, "Computer Science");

    // Budowanie panelu studenta
    studentSizer->Add(new wxStaticText(studentPanel, wxID_ANY, "First Name:"));
    studentSizer->Add(firstNameCtrl, 0, wxEXPAND);
    studentSizer->Add(new wxStaticText(studentPanel, wxID_ANY, "Last Name:"));
    studentSizer->Add(lastNameCtrl, 0, wxEXPAND);
    studentSizer->Add(new wxStaticText(studentPanel, wxID_ANY, "Email:"));
    studentSizer->Add(emailCtrl, 0, wxEXPAND);
    studentSizer->Add(new wxStaticText(studentPanel, wxID_ANY, "Student ID:"));
    studentSizer->Add(studentIdCtrl, 0, wxEXPAND);
    studentSizer->Add(new wxStaticText(studentPanel, wxID_ANY, "Group:"));
    studentSizer->Add(groupCtrl, 0, wxEXPAND);
    studentPanel->SetSizer(studentSizer);

    // Budowanie panelu nauczyciela
    teacherSizer->Add(new wxStaticText(teacherPanel, wxID_ANY, "First Name:"));
    teacherSizer->Add(new wxTextCtrl(teacherPanel, wxID_ANY, "Jane"), 0, wxEXPAND);
    teacherSizer->Add(new wxStaticText(teacherPanel, wxID_ANY, "Last Name:"));
    teacherSizer->Add(new wxTextCtrl(teacherPanel, wxID_ANY, "Smith"), 0, wxEXPAND);
    teacherSizer->Add(new wxStaticText(teacherPanel, wxID_ANY, "Email:"));
    teacherSizer->Add(new wxTextCtrl(teacherPanel, wxID_ANY, "jane.smith@example.com"), 0, wxEXPAND);
    teacherSizer->Add(new wxStaticText(teacherPanel, wxID_ANY, "Teacher ID:"));
    teacherSizer->Add(teacherIdCtrl, 0, wxEXPAND);
    teacherSizer->Add(new wxStaticText(teacherPanel, wxID_ANY, "Department:"));
    teacherSizer->Add(departmentCtrl, 0, wxEXPAND);
    teacherPanel->SetSizer(teacherSizer);

    notebook->AddPage(studentPanel, "Student");
    notebook->AddPage(teacherPanel, "Teacher");

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(&dlg, wxID_OK, "Save"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(&dlg, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    dlg.SetSizer(mainSizer);
    dlg.ShowModal();
}




