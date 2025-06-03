#include "Students.h"
#include <fstream>
#include <algorithm>

// Statyczne pole: œcie¿ka do pliku
const string Students::filePath = "students.json";

// Konstruktor z parametrami
Students::Students(const string& id, const string& first_name, const string& last_name,
    const string& student_id, const string& email, const string& major,
    int year, const string& group, const vector<string>& grades)
    : id(id), first_name(first_name), last_name(last_name), student_id(student_id),
    email(email), major(major), year(year), group(group), grades(grades) {}

// Destruktor
Students::~Students() {}

// Dodanie oceny
void Students::addGrade(const string& grade_id) {
    grades.push_back(grade_id);

    vector<Students> students = loadStudentsFromFile();
    for (auto& student : students) {
        if (student.id == id) {
            student.grades.push_back(grade_id);
            break;
        }
    }
    saveStudentsToFile(students);
}

// Usuniêcie oceny
void Students::removeGrade(const string& grade_id) {
    grades.erase(remove(grades.begin(), grades.end(), grade_id), grades.end());

    vector<Students> students = loadStudentsFromFile();
    for (auto& student : students) {
        if (student.id == id) {
            student.grades.erase(remove(student.grades.begin(), student.grades.end(), grade_id), student.grades.end());
            break;
        }
    }
    saveStudentsToFile(students);
}

// Wyœwietlenie listy ocen
void Students::listGrades() const {
    cout << "Oceny studenta " << first_name << " " << last_name << ":\n";
    for (const auto& grade : grades) {
        cout << "- " << grade << endl;
    }
}

// Wyœwietlenie informacji o studencie
void Students::displayStudentInfo() const {
    cout << "Informacje o studencie:\n";
    cout << "ID: " << id << endl;
    cout << "Imie: " << first_name << endl;
    cout << "Nazwisko: " << last_name << endl;
    cout << "Numer studenta: " << student_id << endl;
    cout << "Email: " << email << endl;
    cout << "Kierunek: " << major << endl;
    cout << "Rok: " << year << endl;
    cout << "Grupa: " << group << endl;
    listGrades();
}

// Wczytanie studentów z pliku JSON
vector<Students> Students::loadStudentsFromFile() {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << filePath << endl;
        return {};
    }

    json studentsData;
    file >> studentsData;

    vector<Students> studentsList;
    for (const auto& studentJson : studentsData) {
        Students student(
            studentJson["id"],
            studentJson["first_name"],
            studentJson["last_name"],
            studentJson["student_id"],
            studentJson["email"],
            studentJson["major"],
            studentJson["year"],
            studentJson["group"],
            studentJson["grades"].get<vector<string>>()
        );
        studentsList.push_back(student);
    }

    return studentsList;
}

// Zapisanie studentów do pliku JSON
void Students::saveStudentsToFile(const vector<Students>& students) {
    json studentsData;
    for (const auto& student : students) {
        studentsData.push_back({
            {"id", student.id},
            {"first_name", student.first_name},
            {"last_name", student.last_name},
            {"student_id", student.student_id},
            {"email", student.email},
            {"major", student.major},
            {"year", student.year},
            {"group", student.group},
            {"grades", student.grades}
            });
    }

    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mozna zapisac do pliku: " << filePath << endl;
        return;
    }

    file << studentsData.dump(4); // Zapisz dane w formacie JSON z wciêciami
}

