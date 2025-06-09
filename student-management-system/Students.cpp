#include "Students.h"
#include <fstream>
#include <algorithm>
#include "bcrypt.h"

const string Students::filePath = "students.json"; 

Students::Students(const string& id, const string& first_name, const string& last_name,
    const string& student_id, const string& email, const string& major,
    int year, const string& group, const vector<string>& grades)
    : id(id), first_name(first_name), last_name(last_name), student_id(student_id),
    email(email), major(major), year(year), group(group), grades(grades) {}

Students::~Students() {}

// Dodanie oceny do studenta
void Students::addGrade(const string& grade_id) {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); 

    auto it = find_if(allGrades.begin(), allGrades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });

    if (it != allGrades.end()) {
        grades.push_back(grade_id); 
        cout << "Dodano ocene o ID: " << grade_id << " dla studenta " << first_name << " " << last_name << endl;

        vector<Students> students = loadStudentsFromFile();
        for (auto& student : students) {
            if (student.id == id) {
                student.grades = grades; 
                break;
            }
        }
        saveStudentsToFile(students); 
    }
    else {
        cerr << "Nie znaleziono oceny o ID: " << grade_id << endl;
    }
}

void Students::removeGrade(const string& grade_id) {
    auto it = find(grades.begin(), grades.end(), grade_id);

    if (it != grades.end()) {
        grades.erase(it); 
        cout << "Usuniêto ocenê o ID: " << grade_id << " dla studenta " << first_name << " " << last_name << endl;

        vector<Students> students = loadStudentsFromFile();
        for (auto& student : students) {
            if (student.id == id) {
                student.grades = grades; 
                break;
            }
        }
        saveStudentsToFile(students); 
    }
    else {
        cerr << "Nie znaleziono oceny o ID: " << grade_id << " w liœcie ocen studenta." << endl;
    }
}

// testowanie ocen studenta
void Students::listGrades() const {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); 

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

// testowanie informacji o studencie
void Students::displayStudentInfo() const {
    cout << "ID: " << id << "\nImiê: " << first_name << "\nNazwisko: " << last_name
        << "\nNumer studenta: " << student_id << "\nEmail: " << email
        << "\nKierunek: " << major << "\nRok: " << year << "\nGrupa: " << group << endl;
    listGrades(); 
    cout << "\n==================\n";
}

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

void Students::saveStudentsToFile(const vector<Students>& students) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << filePath << endl;
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

    file << studentsData.dump(4); 
}

Grade Students::getGrade(const string& grade_id) const {
    vector<Grade> allGrades = Grade::loadGradesFromFile(); 

    auto it = find_if(allGrades.begin(), allGrades.end(), [&grade_id](const Grade& grade) {
        return grade.id == grade_id;
        });

    if (it != allGrades.end()) {
        return *it; 
    }
    else {
        throw runtime_error("Nie znaleziono oceny o ID: " + grade_id);
    }
}

void Students::addStudent(const Students& newStudent, const string& password) {
  
    vector<Students> students = loadStudentsFromFile();

    auto it = find_if(students.begin(), students.end(), [&newStudent](const Students& student) {
        return student.id == newStudent.id;
        });

    if (it != students.end()) {
        cerr << "Student o podanym ID juz istnieje." << endl;
        return;
    }

    students.push_back(newStudent);
    saveStudentsToFile(students); 
    cout << "Dodano nowego studenta: " << newStudent.first_name << " " << newStudent.last_name << endl;

    ifstream usersFile("users.json");
    json usersData;

    if (usersFile.is_open()) {
        usersFile >> usersData;
        usersFile.close();
    }
    else {
        cerr << "Nie mozna otworzyc pliku users.json" << endl;
        return;
    }

    for (const auto& user : usersData) {
        if (user["login"] == newStudent.email) {
            cerr << "Uzytkownik o podanym loginie (email) juz istnieje." << endl;
            return;
        }
    }

    string hashedPassword = bcrypt::generateHash(password);

    json newUser = {
        {"login", newStudent.email},
        {"password", hashedPassword},
        {"role", "student"}
    };
    usersData.push_back(newUser);

    ofstream usersOutFile("users.json");
    if (usersOutFile.is_open()) {
        usersOutFile << usersData.dump(4); 
        usersOutFile.close();
    }
    else {
        cerr << "Nie mozna zapisc do pliku users.json" << endl;
        return;
    }

    cout << "Dodano nowego uzytkownika o loginie: " << newStudent.email << endl;
}

void Students::removeStudent(const string& student_id) {
    vector<Students> students = loadStudentsFromFile(); 
    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id;
        });

    if (it != students.end()) {
        
        vector<Grade> allGrades = Grade::loadGradesFromFile(); 
        allGrades.erase(remove_if(allGrades.begin(), allGrades.end(), [&it](const Grade& grade) {
            return find(it->grades.begin(), it->grades.end(), grade.id) != it->grades.end();
            }), allGrades.end());

        Grade::saveGradesToFile(allGrades); 

        ifstream usersFile("users.json");
        json usersData;

        if (usersFile.is_open()) {
            usersFile >> usersData;
            usersFile.close();
        }
        else {
            cerr << "Nie mo¿na otworzyc pliku users.json" << endl;
            return;
        }

        auto userIt = find_if(usersData.begin(), usersData.end(), [&it](const json& user) {
            return user["login"] == it->email;
            });

        if (userIt != usersData.end()) {
            usersData.erase(userIt);
            ofstream usersOutFile("users.json");
            if (usersOutFile.is_open()) {
                usersOutFile << usersData.dump(4); 
                usersOutFile.close();
                cout << "Usuniêto uzytkownika o loginie: " << it->email << endl;
            }
            else {
                cerr << "Nie mozna zapisac do pliku users.json" << endl;
                return;
            }
        }
        else {
            cerr << "Nie znaleziono uzytkownika o loginie: " << it->email << endl;
        }

        cout << "Usuniêto studenta: " << it->first_name << " " << it->last_name << endl;
        students.erase(it); 
        saveStudentsToFile(students); 
    }
    else {
        cerr << "Nie znaleziono studenta o ID: " << student_id << endl;
    }
}

Students Students::getStudentById(const string& student_id) {
    vector<Students> students = loadStudentsFromFile(); 

    auto it = find_if(students.begin(), students.end(), [&student_id](const Students& student) {
        return student.id == student_id;
        });

    if (it != students.end()) {
        return *it; 
    }
    else {
        throw runtime_error("Nie znaleziono studenta o ID: " + student_id);
    }
}