#include "Exam.h"
#include <fstream>
#include <algorithm>
#include "Teacher.h"

// Œcie¿ka do pliku JSON
const string Exam::filePath = "exams.json";

// Konstruktor
Exam::Exam(const string& id, const string& date, const string& comment, const string& subject, const string& teacher_id)
    : id(id), date(date), comment(comment), subject(subject), teacher_id(teacher_id) {}

// Destruktor
Exam::~Exam() {}

// Wczytanie egzaminów z pliku
vector<Exam> Exam::loadExamsFromFile() {
    ifstream file(filePath);
    vector<Exam> exams;

    if (file.is_open()) {
        json data;
        file >> data;

        for (const auto& item : data) {
            exams.emplace_back(
                item["id"].get<string>(),
                item["date"].get<string>(),
                item["comment"].get<string>(),
                item["subject"].get<string>(),
                item["teacher_id"].get<string>()
            );
        }
    }

    return exams;
}

// Zapisanie egzaminów do pliku
void Exam::saveExamsToFile(const vector<Exam>& exams) {
    ofstream file(filePath);

    if (file.is_open()) {
        json data = json::array();

        for (const auto& exam : exams) {
            data.push_back({
                {"id", exam.id},
                {"date", exam.date},
                {"comment", exam.comment},
                {"subject", exam.subject},
                {"teacher_id", exam.teacher_id}
                });
        }

        file << data.dump(4); // Zapisanie w formacie JSON z wciêciami
    }
}

// Dodanie egzaminu
void Exam::addExam(const Exam& newExam) {
    vector<Exam> exams = loadExamsFromFile();
    exams.push_back(newExam);
    saveExamsToFile(exams);

    // Dodanie ID egzaminu do listy egzaminów nauczyciela
    vector<Teacher> teachers = Teacher::loadTeachersFromFile();
    auto it = find_if(teachers.begin(), teachers.end(), [&newExam](const Teacher& teacher) {
        return teacher.id == newExam.teacher_id;
        });

    if (it != teachers.end()) {
        it->exams.push_back(newExam.id); // Dodanie ID egzaminu do nauczyciela
        Teacher::saveTeachersToFile(teachers); // Zapisanie zaktualizowanych danych nauczycieli
    }
    else {
        cerr << "Nie znaleziono nauczyciela o ID: " << newExam.teacher_id << endl;
    }

    cout << "Dodano egzamin o ID: " << newExam.id << endl;
}


// Usuniêcie egzaminu
void Exam::removeExam(const string& exam_id) {
    vector<Exam> exams = loadExamsFromFile();
    auto it = remove_if(exams.begin(), exams.end(), [&exam_id](const Exam& exam) {
        return exam.id == exam_id;
        });

    if (it != exams.end()) {
        string teacher_id = it->teacher_id; // Pobranie ID nauczyciela przed usuniêciem egzaminu
        exams.erase(it, exams.end());
        saveExamsToFile(exams);

        // Usuniêcie ID egzaminu z listy egzaminów nauczyciela
        vector<Teacher> teachers = Teacher::loadTeachersFromFile();
        auto teacherIt = find_if(teachers.begin(), teachers.end(), [&teacher_id](const Teacher& teacher) {
            return teacher.id == teacher_id;
            });

        if (teacherIt != teachers.end()) {
            auto examIt = find(teacherIt->exams.begin(), teacherIt->exams.end(), exam_id);
            if (examIt != teacherIt->exams.end()) {
                teacherIt->exams.erase(examIt); // Usuniêcie ID egzaminu z listy nauczyciela
                Teacher::saveTeachersToFile(teachers); // Zapisanie zaktualizowanych danych nauczycieli
            }
        }
        else {
            cerr << "Nie znaleziono nauczyciela o ID: " << teacher_id << endl;
        }

        cout << "Usuniêto egzamin o ID: " << exam_id << endl;
    }
    else {
        cerr << "Nie znaleziono egzaminu o ID: " << exam_id << endl;
    }
}


// Pobranie egzaminu po ID
Exam Exam::getExamById(const string& exam_id) {
    vector<Exam> exams = loadExamsFromFile();
    auto it = find_if(exams.begin(), exams.end(), [&exam_id](const Exam& exam) {
        return exam.id == exam_id;
        });

    if (it != exams.end()) {
        return *it;
    }
    else {
        throw runtime_error("Nie znaleziono egzaminu o ID: " + exam_id);
    }
}

// Wyœwietlenie informacji o egzaminie
void Exam::displayExamInfo() const {
    cout << "ID: " << id << "\n"
        << "Data: " << date << "\n"
        << "Komentarz: " << comment << "\n"
        << "Przedmiot: " << subject << "\n"
        << "ID nauczyciela: " << teacher_id << endl;
}
