#include "Students.h"
#include <fstream>
#include <algorithm>
#include "bcrypt.h"

const string Students::filePath = "students.json"; // �cie�ka do pliku JSON

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
        cout << "Dodano ocen� o ID: " << grade_id << " dla studenta " << first_name << " " << last_name << endl;

        // Wczytanie wszystkich student�w, aktualizacja i zapis do pliku
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

// Usuni�cie oceny ze studenta
void Students::removeGrade(const string& grade_id) {
    auto it = find(grades.begin(), grades.end(), grade_id);

    if (it != grades.end()) {
        grades.erase(it); // Usuni�cie ID oceny z listy
        cout << "Usuni�to ocen� o ID: " << grade_id << " dla studenta " << first_name << " " << last_name << endl;

        // Wczytanie wszystkich student�w, aktualizacja i zapis do pliku
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
        cerr << "Nie znaleziono oceny o ID: " << grade_id << " w li�cie ocen studenta." << endl;
    }
}

// Wy�wietlenie ocen studenta
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

// Wy�wietlenie informacji o studencie
void Students::displayStudentInfo() const {
    cout << "ID: " << id << "\nImi�: " << first_name << "\nNazwisko: " << last_name
        << "\nNumer studenta: " << student_id << "\nEmail: " << email
        << "\nKierunek: " << major << "\nRok: " << year << "\nGrupa: " << group << endl;
    listGrades(); // Wy�wietlenie ocen studenta
    cout << "\n==================\n";
}

// Wczytanie student�w z pliku JSON
vector<Students> Students::loadStudentsFromFile() {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mo�na otworzy� pliku: " << filePath << endl;
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

// Zapisanie student�w do pliku JSON
void Students::saveStudentsToFile(const vector<Students>& students) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mo�na otworzy� pliku: " << filePath << endl;
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

    file << studentsData.dump(4); // Zapisanie w formacie JSON z wci�ciami
}

// Pobranie obiektu Grade na podstawie ID
Grade Students::getGrade(const string& grade_id) const {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); // Wczytanie wszystkich ocen z pliku JSON

    auto it = find_if(allGrades.begin(), allGrades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });

    if (it != allGrades.end()) {
        return *it; // Zwr�cenie znalezionego obiektu Grade
    }
    else {
        throw runtime_error("Nie znaleziono oceny o ID: " + grade_id);
    }
}

// Dodanie nowego studenta
void Students::addStudent(const Students& newStudent, const string& password) {
    // Wczytanie wszystkich student�w z pliku
    vector<Students> students = loadStudentsFromFile();

    // Sprawdzenie, czy student o podanym ID ju� istnieje
    auto it = find_if(students.begin(), students.end(), [&newStudent](const Students& student) {
        return student.id == newStudent.id;
        });

    if (it != students.end()) {
        cerr << "Student o podanym ID ju� istnieje." << endl;
        return;
    }

    // Dodanie nowego studenta do listy
    students.push_back(newStudent);
    saveStudentsToFile(students); // Zapisanie zaktualizowanej listy student�w do pliku
    cout << "Dodano nowego studenta: " << newStudent.first_name << " " << newStudent.last_name << endl;

    // Wczytanie bazy u�ytkownik�w
    ifstream usersFile("users.json");
    json usersData;

    if (usersFile.is_open()) {
        usersFile >> usersData;
        usersFile.close();
    }
    else {
        cerr << "Nie mo�na otworzy� pliku users.json" << endl;
        return;
    }

    // Sprawdzenie, czy u�ytkownik o podanym loginie (email) ju� istnieje
    for (const auto& user : usersData) {
        if (user["login"] == newStudent.email) {
            cerr << "U�ytkownik o podanym loginie (email) ju� istnieje." << endl;
            return;
        }
    }

    // Hashowanie has�a
    string hashedPassword = bcrypt::generateHash(password);

    // Dodanie nowego u�ytkownika do bazy
    json newUser = {
        {"login", newStudent.email},
        {"password", hashedPassword},
        {"role", "student"}
    };
    usersData.push_back(newUser);

    // Zapisanie zaktualizowanej bazy u�ytkownik�w
    ofstream usersOutFile("users.json");
    if (usersOutFile.is_open()) {
        usersOutFile << usersData.dump(4); // Zapisanie w formacie JSON z wci�ciami
        usersOutFile.close();
    }
    else {
        cerr << "Nie mo�na zapisa� do pliku users.json" << endl;
        return;
    }

    cout << "Dodano nowego u�ytkownika o loginie: " << newStudent.email << endl;
}

// Usuni�cie studenta (wraz z jego ocenami)
void Students::removeStudent(const string& student_id) {
    vector<Students> students = loadStudentsFromFile(); // Wczytanie wszystkich student�w z pliku
    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id;
        });

    if (it != students.end()) {
        // Usuni�cie ocen studenta
        vector<Grade> allGrades = Grade::loadGradesFromFile(); // Wczytanie wszystkich ocen z pliku JSON
        allGrades.erase(remove_if(allGrades.begin(), allGrades.end(), [&it](const Grade& grade) {
            return find(it->grades.begin(), it->grades.end(), grade.id) != it->grades.end();
            }), allGrades.end());

        Grade::saveGradesToFile(allGrades); // Zapisanie zaktualizowanej listy ocen

        // Usuni�cie u�ytkownika z bazy `users.json`
        ifstream usersFile("users.json");
        json usersData;

        if (usersFile.is_open()) {
            usersFile >> usersData;
            usersFile.close();
        }
        else {
            cerr << "Nie mo�na otworzy� pliku users.json" << endl;
            return;
        }

        // Znalezienie i usuni�cie u�ytkownika na podstawie emaila studenta
        auto userIt = find_if(usersData.begin(), usersData.end(), [&it](const json& user) {
            return user["login"] == it->email;
            });

        if (userIt != usersData.end()) {
            usersData.erase(userIt); // Usuni�cie u�ytkownika
            ofstream usersOutFile("users.json");
            if (usersOutFile.is_open()) {
                usersOutFile << usersData.dump(4); // Zapisanie zaktualizowanej bazy u�ytkownik�w
                usersOutFile.close();
                cout << "Usuni�to u�ytkownika o loginie: " << it->email << endl;
            }
            else {
                cerr << "Nie mo�na zapisa� do pliku users.json" << endl;
                return;
            }
        }
        else {
            cerr << "Nie znaleziono u�ytkownika o loginie: " << it->email << endl;
        }

        // Usuni�cie studenta
        cout << "Usuni�to studenta: " << it->first_name << " " << it->last_name << endl;
        students.erase(it); // Usuni�cie studenta z listy
        saveStudentsToFile(students); // Zapisanie zaktualizowanej listy student�w do pliku
    }
    else {
        cerr << "Nie znaleziono studenta o ID: " << student_id << endl;
    }
}

Students Students::getStudentById(const string& student_id) {
    vector<Students> students = loadStudentsFromFile(); // Wczytanie wszystkich student�w z pliku

    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id;
        });

    if (it != students.end()) {
        return *it; // Zwr�cenie znalezionego studenta
    }
    else {
        throw runtime_error("Nie znaleziono studenta o ID: " + student_id);
    }
}