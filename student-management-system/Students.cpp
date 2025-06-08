#include "Students.h"
#include <fstream>
#include <algorithm>

const string Students::filePath = "students.json"; // Œcie¿ka do pliku JSON

// Konstruktor
Students::Students(const string& id, const string& first_name, const string& last_name,
    const string& student_id, const string& email, const string& major,
    int year, const string& group, const vector<string>& grades)
    : id(id), first_name(first_name), last_name(last_name), student_id(student_id),
    email(email), major(major), year(year), group(group), grades(grades) {}

// Destruktor
Students::~Students() {}

// Dodanie oceny do studenta
void Students::addGrade(const string& grade_id) {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); // Wczytanie wszystkich ocen z pliku JSON

    auto it = find_if(allGrades.begin(), allGrades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });

    if (it != allGrades.end()) {
        grades.push_back(grade_id); // Dodanie ID oceny do listy
        cout << "Dodano ocenê o ID: " << grade_id << " dla studenta " << first_name << " " << last_name << endl;

        // Wczytanie wszystkich studentów, aktualizacja i zapis do pliku
        vector<Students> students = loadStudentsFromFile();
        for (auto& student : students) {
            if (student.id == id) {
                student.grades = grades; // Aktualizacja listy ocen
                break;
            }
        }
        saveStudentsToFile(students); // Zapisanie zaktualizowanych danych do pliku
    }
    else {
        cerr << "Nie znaleziono oceny o ID: " << grade_id << endl;
    }
}

// Usuniêcie oceny ze studenta
void Students::removeGrade(const string& grade_id) {
    auto it = find(grades.begin(), grades.end(), grade_id);

    if (it != grades.end()) {
        grades.erase(it); // Usuniêcie ID oceny z listy
        cout << "Usuniêto ocenê o ID: " << grade_id << " dla studenta " << first_name << " " << last_name << endl;

        // Wczytanie wszystkich studentów, aktualizacja i zapis do pliku
        vector<Students> students = loadStudentsFromFile();
        for (auto& student : students) {
            if (student.id == id) {
                student.grades = grades; // Aktualizacja listy ocen
                break;
            }
        }
        saveStudentsToFile(students); // Zapisanie zaktualizowanych danych do pliku
    }
    else {
        cerr << "Nie znaleziono oceny o ID: " << grade_id << " w liœcie ocen studenta." << endl;
    }
}

// Wyœwietlenie ocen studenta
void Students::listGrades() const {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); // Wczytanie wszystkich ocen z pliku JSON

    cout << "\nOceny studenta " << first_name << " " << last_name << ":" << endl;

    for (const auto& gradeId : grades) {
        auto it = find_if(allGrades.begin(), allGrades.end(), [&gradeId](const Grade& grade) {
            return grade.id == gradeId;
            });

        if (it != allGrades.end()) {
            cout << "  ID: " << it->id << "\n  Ocena: " << it->grade
                << "\n  Przedmiot: " << it->subject << "\n  Komentarz: " << it->comment
                << "\n  Data: " << it->date << endl << "+++";
        }
        else {
            cout << "Nie znaleziono oceny o ID: " << gradeId << endl;
        }
    }
}

// Wyœwietlenie informacji o studencie
void Students::displayStudentInfo() const {
    cout << "ID: " << id << "\nImiê: " << first_name << "\nNazwisko: " << last_name
        << "\nNumer studenta: " << student_id << "\nEmail: " << email
        << "\nKierunek: " << major << "\nRok: " << year << "\nGrupa: " << group << endl;
    listGrades(); // Wyœwietlenie ocen studenta
    cout << "\n==================\n";
}

// Wczytanie studentów z pliku JSON
vector<Students> Students::loadStudentsFromFile() {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mo¿na otworzyæ pliku: " << filePath << endl;
        return {};
    }

    json studentsData;
    file >> studentsData;

    vector<Students> studentsList;
    for (const auto& studentJson : studentsData) {
        Students student(
            studentJson["id"].get<string>(),
            studentJson["first_name"].get<string>(),
            studentJson["last_name"].get<string>(),
            studentJson["student_id"].get<string>(),
            studentJson["email"].get<string>(),
            studentJson["major"].get<string>(),
            studentJson["year"].get<int>(),
            studentJson["group"].get<string>(),
            studentJson["grades"].get<vector<string>>()
        );
        studentsList.push_back(student);
    }

    return studentsList;
}

// Zapisanie studentów do pliku JSON
void Students::saveStudentsToFile(const vector<Students>& students) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mo¿na otworzyæ pliku: " << filePath << endl;
        return;
    }

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

    file << studentsData.dump(4); // Zapisanie w formacie JSON z wciêciami
}

// Pobranie obiektu Grade na podstawie ID
Grade Students::getGrade(const string& grade_id) const {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); // Wczytanie wszystkich ocen z pliku JSON

    auto it = find_if(allGrades.begin(), allGrades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });

    if (it != allGrades.end()) {
        return *it; // Zwrócenie znalezionego obiektu Grade
    }
    else {
        throw runtime_error("Nie znaleziono oceny o ID: " + grade_id);
    }
}

// Dodanie nowego studenta
void Students::addStudent(const Students& newStudent) {
    vector<Students> students = loadStudentsFromFile(); // Wczytanie wszystkich studentów z pliku
    students.push_back(newStudent); // Dodanie nowego studenta do listy
    saveStudentsToFile(students); // Zapisanie zaktualizowanej listy do pliku
    cout << "Dodano nowego studenta: " << newStudent.first_name << " " << newStudent.last_name << endl;
}

// Usuniêcie studenta (wraz z jego ocenami)
void Students::removeStudent(const string& student_id) {
    vector<Students> students = loadStudentsFromFile(); // Wczytanie wszystkich studentów z pliku
    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id;
        });

    if (it != students.end()) {
        // Usuniêcie ocen studenta
        vector<Grade> allGrades = Grade::loadGradesFromFile(); // Wczytanie wszystkich ocen z pliku JSON
        allGrades.erase(remove_if(allGrades.begin(), allGrades.end(), [&it](const Grade& grade) {
            return find(it->grades.begin(), it->grades.end(), grade.id) != it->grades.end();
            }), allGrades.end());

        Grade::saveGradesToFile(allGrades); // Zapisanie zaktualizowanej listy ocen

        // Usuniêcie studenta
        cout << "Usuniêto studenta: " << it->first_name << " " << it->last_name << endl;
        students.erase(it); // Usuniêcie studenta z listy
        saveStudentsToFile(students); // Zapisanie zaktualizowanej listy studentów do pliku
    }
    else {
        cerr << "Nie znaleziono studenta o ID: " << student_id << endl;
    }
}

Students Students::getStudentById(const string& student_id) {
    vector<Students> students = loadStudentsFromFile(); // Wczytanie wszystkich studentów z pliku

    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id;
        });

    if (it != students.end()) {
        return *it; // Zwrócenie znalezionego studenta
    }
    else {
        throw runtime_error("Nie znaleziono studenta o ID: " + student_id);
    }
}