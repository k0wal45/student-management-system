#include "Teacher.h"
#include <fstream>
#include <algorithm>
#include "Grade.h"
#include "Students.h"
#include "bcrypt.h"

const string Teacher::filePath = "teachers.json";

Teacher::Teacher(const string& id, const string& first_name, const string& last_name,
    const string& subject, const string& email, const vector<string>& grades, const vector<string>& exams)
    : id(id), first_name(first_name), last_name(last_name), subject(subject), email(email), grades(grades), exams(exams) {}

Teacher::~Teacher() {}

void Teacher::addGrade(const string& student_id, const Grade& grade) {
    // Wczytanie listy studentów z pliku
    vector<Students> students = Students::loadStudentsFromFile();

    // Znalezienie studenta po ID
    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id; 
        });

    if (it != students.end()) {

        it->addGrade(grade.id);

        Students::saveStudentsToFile(students);

        vector<Grade> grades = Grade::loadGradesFromFile();

        grades.push_back(grade);

        Grade::saveGradesToFile(grades);

        this->grades.push_back(grade.id);

        vector<Teacher> teachers = Teacher::loadTeachersFromFile();
        for (auto& teacher : teachers) {
            if (teacher.id == this->id) {
                teacher.grades = this->grades; 
                break;
            }
        }
        Teacher::saveTeachersToFile(teachers);

        cout << "Dodano ocene o ID: " << grade.id << " studentowi o ID: " << student_id << " i nauczycielowi." << endl;
    }
    else {
        cerr << "Nie znaleziono studenta o ID: " << student_id << endl;
    }
}

// Usuniêcie oceny
void Teacher::removeGrade(const string& grade_id) {

    vector<Students> students = Students::loadStudentsFromFile();

    for (auto& student : students) {
        auto it = find(student.grades.begin(), student.grades.end(), grade_id);
        if (it != student.grades.end()) {
            student.removeGrade(grade_id); 
            break;
        }
    }

    Students::saveStudentsToFile(students);

    vector<Grade> grades = Grade::loadGradesFromFile();

    auto gradeIt = find_if(grades.begin(), grades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });
    if (gradeIt != grades.end()) {
        grades.erase(gradeIt); 
    }

    Grade::saveGradesToFile(grades);

    auto teacherGradeIt = find(this->grades.begin(), this->grades.end(), grade_id);
    if (teacherGradeIt != this->grades.end()) {
        this->grades.erase(teacherGradeIt);
    }

    vector<Teacher> teachers = Teacher::loadTeachersFromFile();
    for (auto& teacher : teachers) {
        if (teacher.id == this->id) {
            teacher.grades = this->grades; 
            break;
        }
    }
    Teacher::saveTeachersToFile(teachers);

    cout << "Ocena usunieta o ID: " << grade_id << endl;
}



// Dodanie egzaminu
void Teacher::addExam(const string& student_id, const Exam& exam) {

    vector<Exam> exams = Exam::loadExamsFromFile();

    exams.push_back(exam);

    Exam::saveExamsToFile(exams);

    this->exams.push_back(exam.id);

    vector<Teacher> teachers = Teacher::loadTeachersFromFile();
    for (auto& teacher : teachers) {
        if (teacher.id == this->id) {
            teacher.exams = this->exams;
            break;
        }
    }
    Teacher::saveTeachersToFile(teachers);

    cout << "Dodano egzamin o ID: " << exam.id << " do nauczyciela o ID: " << this->id << endl;
}

// Usuniêcie egzaminu
void Teacher::removeExam(const string& exam_id) {

    vector<Exam> exams = Exam::loadExamsFromFile();

    auto examIt = remove_if(exams.begin(), exams.end(), [&exam_id](const Exam& exam) {
        return exam.id == exam_id;
        });

    if (examIt != exams.end()) {
        exams.erase(examIt, exams.end());
        Exam::saveExamsToFile(exams);

        auto teacherExamIt = find(this->exams.begin(), this->exams.end(), exam_id);
        if (teacherExamIt != this->exams.end()) {
            this->exams.erase(teacherExamIt);
        }

        vector<Teacher> teachers = Teacher::loadTeachersFromFile();
        for (auto& teacher : teachers) {
            if (teacher.id == this->id) {
                teacher.exams = this->exams; 
                break;
            }
        }
        Teacher::saveTeachersToFile(teachers);

        cout << "Usunieto egzamin o ID: " << exam_id << " z nauczyciela o ID: " << this->id << endl;
    }
    else {
        cerr << "Nie znaleziono egzaminu o ID: " << exam_id << endl;
    }
}

// Wyœwietlenie informacji o nauczycielu
void Teacher::displayTeacherInfo() const {
    cout << "ID: " << id << "\n"
        << "Imie: " << first_name << "\n"
        << "Nazwisko: " << last_name << "\n"
        << "Przedmiot: " << subject << "\n"
        << "Oceny: ";
    for (const auto& grade : grades) {
        cout << grade << " ";
    }
    cout << "\nEgzaminy: ";
    for (const auto& exam : exams) {
        cout << exam << " ";
    }
    cout << endl;
}

// Wczytanie nauczycieli z pliku
vector<Teacher> Teacher::loadTeachersFromFile() {
    ifstream file(filePath);
    vector<Teacher> teachers;

    if (file.is_open()) {
        json data;
        file >> data;

        for (const auto& item : data) {
            teachers.emplace_back(
                item["id"].get<string>(),
                item["first_name"].get<string>(),
                item["last_name"].get<string>(),
                item["subject"].get<string>(),
                item["email"].get<string>(),
                item["grades"].get<vector<string>>(),
                item["exams"].get<vector<string>>()
            );
        }
    }

    return teachers;
}

// Zapisanie nauczycieli do pliku
void Teacher::saveTeachersToFile(const vector<Teacher>& teachers) {
    ofstream file(filePath);

    if (file.is_open()) {
        json data = json::array();

        for (const auto& teacher : teachers) {
            data.push_back({
                {"id", teacher.id},
                {"first_name", teacher.first_name},
                {"last_name", teacher.last_name},
                {"subject", teacher.subject},
                {"email", teacher.email},
                {"grades", teacher.grades},
                {"exams", teacher.exams}
                });
        }

        file << data.dump(4);
    }
}

// Dodanie nauczyciela
void Teacher::addTeacher(const Teacher& newTeacher, const string& password) {
    
    vector<Teacher> teachers = loadTeachersFromFile();

    teachers.push_back(newTeacher);

    saveTeachersToFile(teachers);

    // Dodanie nowego u¿ytkownika do pliku `users.json`
    ifstream usersFile("users.json");
    json usersData;

    if (usersFile.is_open()) {
        usersFile >> usersData;
        usersFile.close();
    }
    else {
        cerr << "Nie mo¿na otworzyæ pliku users.json" << endl;
        return;
    }

    // Sprawdzenie, czy u¿ytkownik z takim loginem ju¿ istnieje
    auto userIt = find_if(usersData.begin(), usersData.end(), [&newTeacher](const json& user) {
        return user["login"] == newTeacher.email;
        });

	string hashedPassword = bcrypt::generateHash(password); 

    if (userIt == usersData.end()) {
        // Dodanie nowego u¿ytkownika
        json newUser = {
            {"login", newTeacher.email},
            {"password", hashedPassword}, 
            {"role", "teacher"}             
        };
        usersData.push_back(newUser);

        // Zapisanie zaktualizowanej bazy u¿ytkowników do pliku
        ofstream usersOutFile("users.json");
        if (usersOutFile.is_open()) {
            usersOutFile << usersData.dump(4);
            usersOutFile.close();
            cout << "Dodano nowego u¿ytkownika o loginie: " << newTeacher.email << endl;
        }
        else {
            cerr << "Nie mo¿na zapisaæ do pliku users.json" << endl;
        }
    }
    else {
        cerr << "U¿ytkownik o loginie: " << newTeacher.email << " ju¿ istnieje w systemie." << endl;
    }

    cout << "Dodano nauczyciela o ID: " << newTeacher.id << endl;
}


// Usuniêcie nauczyciela
void Teacher::removeTeacher(const string& teacher_id) {
    vector<Teacher> teachers = loadTeachersFromFile();
    auto it = remove_if(teachers.begin(), teachers.end(), [&teacher_id](const Teacher& teacher) {
        return teacher.id == teacher_id;
        });

    if (it != teachers.end()) {
        
        vector<string> examsToRemove = it->exams;

        string teacherEmail = it->email;

        teachers.erase(it, teachers.end());
        saveTeachersToFile(teachers);

        vector<Exam> exams = Exam::loadExamsFromFile();

        // Usuniêcie egzaminów powi¹zanych z nauczycielem
        exams.erase(remove_if(exams.begin(), exams.end(), [&examsToRemove](const Exam& exam) {
            return find(examsToRemove.begin(), examsToRemove.end(), exam.id) != examsToRemove.end();
            }), exams.end());

        Exam::saveExamsToFile(exams);

        // Usuniêcie u¿ytkownika z bazy `users.json`
        ifstream usersFile("users.json");
        json usersData;

        if (usersFile.is_open()) {
            usersFile >> usersData;
            usersFile.close();
        }
        else {
            cerr << "Nie mo¿na otworzyæ pliku users.json" << endl;
            return;
        }

        // Znalezienie i usuniêcie u¿ytkownika na podstawie emaila nauczyciela
        auto userIt = find_if(usersData.begin(), usersData.end(), [&teacherEmail](const json& user) {
            return user["login"] == teacherEmail;
            });

        if (userIt != usersData.end()) {
            usersData.erase(userIt);
            ofstream usersOutFile("users.json");
            if (usersOutFile.is_open()) {
                usersOutFile << usersData.dump(4); 
                usersOutFile.close();
                cout << "Usunieto uzytkownika o loginie: " << teacherEmail << endl;
            }
            else {
                cerr << "Nie mozna zapisac do pliku users.json" << endl;
                return;
            }
        }
        else {
            cerr << "Nie znaleziono u¿ytkownika o loginie: " << teacherEmail << endl;
        }

        cout << "Usunieto nauczyciela o ID: " << teacher_id << " oraz powiazane z nim egzaminy." << endl;
    }
    else {
        cerr << "Nie znaleziono nauczyciela o ID: " << teacher_id << endl;
    }
}


// Pobranie nauczyciela po ID
Teacher Teacher::getTeacherById(const string& teacher_id) {
    vector<Teacher> teachers = loadTeachersFromFile();
    auto it = find_if(teachers.begin(), teachers.end(), [&teacher_id](const Teacher& teacher) {
        return teacher.id == teacher_id;
        });

    if (it != teachers.end()) {
        return *it;
    }
    else {
        throw runtime_error("Nie znaleziono nauczyciela o ID: " + teacher_id);
    }
}
