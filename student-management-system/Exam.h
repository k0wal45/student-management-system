#ifndef EXAM_H
#define EXAM_H

#include <string>
#include <vector>
#include <iostream>
#include "json.hpp" // Biblioteka JSON (np. nlohmann/json)

using namespace std;
using json = nlohmann::json;

class Exam {
public:
    // Konstruktor i destruktor
    Exam(const string& id, const string& date, const string& comment, const string& subject, const string& teacher_id);
    ~Exam();

    // Metody publiczne
    static vector<Exam> loadExamsFromFile();
    static void saveExamsToFile(const vector<Exam>& exams);

    static void addExam(const Exam& newExam); // Dodanie egzaminu
    static void removeExam(const string& exam_id); // Usuniêcie egzaminu
    static Exam getExamById(const string& exam_id); // Pobranie egzaminu po ID

    void displayExamInfo() const;

private:
    // Pola prywatne
    string id;
    string date;
    string comment;
    string subject;
    string teacher_id;

    static const string filePath; // Œcie¿ka do pliku JSON
};

#endif // EXAM_H
