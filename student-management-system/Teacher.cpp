#include "Teacher.h"
#include <fstream>
#include <algorithm>
#include "Grade.h"
#include "Students.h"

// �cie�ka do pliku JSON
const string Teacher::filePath = "teachers.json";

// Konstruktor
Teacher::Teacher(const string& id, const string& first_name, const string& last_name,
    const string& subject, const string& email, const vector<string>& grades, const vector<string>& exams)
    : id(id), first_name(first_name), last_name(last_name), subject(subject), email(email), grades(grades), exams(exams) {}

// Destruktor
Teacher::~Teacher() {}

// Implementacja metody addGrade
void Teacher::addGrade(const string& student_id, const Grade& grade) {
    // Wczytanie listy student�w z pliku
    vector<Students> students = Students::loadStudentsFromFile();

    // Znalezienie studenta po ID
    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id; // Por�wnanie ID studenta
        });

    if (it != students.end()) {
        // Dodanie ID oceny do studenta
        it->addGrade(grade.id);

        // Zapisanie zaktualizowanej listy student�w do pliku
        Students::saveStudentsToFile(students);

        // Wczytanie listy ocen z pliku
        vector<Grade> grades = Grade::loadGradesFromFile();

        // Dodanie nowej oceny do listy ocen
        grades.push_back(grade);

        // Zapisanie zaktualizowanej listy ocen do pliku
        Grade::saveGradesToFile(grades);

        // Dodanie ID oceny do listy ocen nauczyciela
        this->grades.push_back(grade.id);

        // Zapisanie zaktualizowanej listy nauczycieli do pliku
        vector<Teacher> teachers = Teacher::loadTeachersFromFile();
        for (auto& teacher : teachers) {
            if (teacher.id == this->id) {
                teacher.grades = this->grades; // Aktualizacja listy ocen nauczyciela
                break;
            }
        }
        Teacher::saveTeachersToFile(teachers);

        cout << "Dodano ocen� o ID: " << grade.id << " studentowi o ID: " << student_id << " i nauczycielowi." << endl;
    }
    else {
        cerr << "Nie znaleziono studenta o ID: " << student_id << endl;
    }
}


void Teacher::removeGrade(const string& grade_id) {
    // Wczytanie listy student�w z pliku
    vector<Students> students = Students::loadStudentsFromFile();

    // Usuni�cie oceny z listy ocen studenta
    for (auto& student : students) {
        auto it = find(student.grades.begin(), student.grades.end(), grade_id);
        if (it != student.grades.end()) {
            student.removeGrade(grade_id); // Usuni�cie oceny z listy ocen studenta
            break;
        }
    }

    // Zapisanie zaktualizowanej listy student�w do pliku
    Students::saveStudentsToFile(students);

    // Wczytanie listy ocen z pliku
    vector<Grade> grades = Grade::loadGradesFromFile();

    // Usuni�cie oceny z globalnej listy ocen
    auto gradeIt = find_if(grades.begin(), grades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });
    if (gradeIt != grades.end()) {
        grades.erase(gradeIt); // Usuni�cie oceny
    }

    // Zapisanie zaktualizowanej listy ocen do pliku
    Grade::saveGradesToFile(grades);

    // Usuni�cie oceny z listy ocen nauczyciela
    auto teacherGradeIt = find(this->grades.begin(), this->grades.end(), grade_id);
    if (teacherGradeIt != this->grades.end()) {
        this->grades.erase(teacherGradeIt);
    }

    // Zapisanie zaktualizowanej listy nauczycieli do pliku
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();
    for (auto& teacher : teachers) {
        if (teacher.id == this->id) {
            teacher.grades = this->grades; // Aktualizacja listy ocen nauczyciela
            break;
        }
    }
    Teacher::saveTeachersToFile(teachers);

    cout << "Usuni�to ocen� o ID: " << grade_id << endl;
}



// Dodanie egzaminu
void Teacher::addExam(const string& student_id, const Exam& exam) {
    // Wczytanie listy egzamin�w z pliku
    vector<Exam> exams = Exam::loadExamsFromFile();

    // Dodanie nowego egzaminu do listy egzamin�w
    exams.push_back(exam);

    // Zapisanie zaktualizowanej listy egzamin�w do pliku
    Exam::saveExamsToFile(exams);

    // Dodanie ID egzaminu do listy egzamin�w nauczyciela
    this->exams.push_back(exam.id);

    // Zapisanie zaktualizowanej listy nauczycieli do pliku
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();
    for (auto& teacher : teachers) {
        if (teacher.id == this->id) {
            teacher.exams = this->exams; // Aktualizacja listy egzamin�w nauczyciela
            break;
        }
    }
    Teacher::saveTeachersToFile(teachers);

    cout << "Dodano egzamin o ID: " << exam.id << " do nauczyciela o ID: " << this->id << endl;
}

// Usuni�cie egzaminu
void Teacher::removeExam(const string& exam_id) {
    // Wczytanie listy egzamin�w z pliku
    vector<Exam> exams = Exam::loadExamsFromFile();

    // Usuni�cie egzaminu z globalnej listy egzamin�w
    auto examIt = remove_if(exams.begin(), exams.end(), [&exam_id](const Exam& exam) {
        return exam.id == exam_id;
        });

    if (examIt != exams.end()) {
        exams.erase(examIt, exams.end());
        Exam::saveExamsToFile(exams);

        // Usuni�cie ID egzaminu z listy egzamin�w nauczyciela
        auto teacherExamIt = find(this->exams.begin(), this->exams.end(), exam_id);
        if (teacherExamIt != this->exams.end()) {
            this->exams.erase(teacherExamIt);
        }

        // Zapisanie zaktualizowanej listy nauczycieli do pliku
        vector<Teacher> teachers = Teacher::loadTeachersFromFile();
        for (auto& teacher : teachers) {
            if (teacher.id == this->id) {
                teacher.exams = this->exams; // Aktualizacja listy egzamin�w nauczyciela
                break;
            }
        }
        Teacher::saveTeachersToFile(teachers);

        cout << "Usuni�to egzamin o ID: " << exam_id << " z nauczyciela o ID: " << this->id << endl;
    }
    else {
        cerr << "Nie znaleziono egzaminu o ID: " << exam_id << endl;
    }
}

// Wy�wietlenie informacji o nauczycielu
void Teacher::displayTeacherInfo() const {
    cout << "ID: " << id << "\n"
        << "Imi�: " << first_name << "\n"
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
void Teacher::addTeacher(const Teacher& newTeacher) {
    vector<Teacher> teachers = loadTeachersFromFile();
    teachers.push_back(newTeacher);
    saveTeachersToFile(teachers);
}

// Usuni�cie nauczyciela
void Teacher::removeTeacher(const string& teacher_id) {
    vector<Teacher> teachers = loadTeachersFromFile();
    auto it = remove_if(teachers.begin(), teachers.end(), [&teacher_id](const Teacher& teacher) {
        return teacher.id == teacher_id;
        });

    if (it != teachers.end()) {
        // Pobranie listy egzamin�w powi�zanych z nauczycielem
        vector<string> examsToRemove = it->exams;

        // Usuni�cie nauczyciela z listy
        teachers.erase(it, teachers.end());
        saveTeachersToFile(teachers);

        // Wczytanie listy egzamin�w z pliku
        vector<Exam> exams = Exam::loadExamsFromFile();

        // Usuni�cie egzamin�w powi�zanych z nauczycielem
        exams.erase(remove_if(exams.begin(), exams.end(), [&examsToRemove](const Exam& exam) {
            return find(examsToRemove.begin(), examsToRemove.end(), exam.id) != examsToRemove.end();
            }), exams.end());

        // Zapisanie zaktualizowanej listy egzamin�w do pliku
        Exam::saveExamsToFile(exams);

        cout << "Usuni�to nauczyciela o ID: " << teacher_id << " oraz powi�zane z nim egzaminy." << endl;
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
