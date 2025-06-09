#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <vector>
#include <iostream>
#include "json.hpp" // Biblioteka JSON (np. nlohmann/json)
#include "Grade.h" 
#include "Exam.h"

using namespace std;
using json = nlohmann::json;

class Teacher {
public:
    string id;
    string first_name;
    string last_name;
    string subject;
    string email;
	// Has³o nauczyciela, mo¿e byæ u¿ywane do logowania
    vector<string> grades; // Lista ID ocen wpisanych przez nauczyciela
    vector<string> exams;  // Lista ID egzaminów stworzonych przez nauczyciela

    static const string filePath; // Œcie¿ka do pliku JSON
    // Konstruktor i destruktor
    Teacher(const string& id, const string& first_name, const string& last_name,
        const string& subject, const string& email, const vector<string>& grades, const vector<string>& exams);
    ~Teacher();

    // Metody publiczne
    void addGrade(const string& student_id, const Grade& grade);
    void removeGrade(const string& grade_id);
    void addExam(const string& student_id, const Exam& exam);
    void removeExam(const string& exam_id);
    void displayTeacherInfo() const;

    static vector<Teacher> loadTeachersFromFile();
    static void saveTeachersToFile(const vector<Teacher>& teachers);

    static void addTeacher(const Teacher& newTeacher, const string& password); // Dodanie nauczyciela
    static void removeTeacher(const string& teacher_id); // Usuniêcie nauczyciela
    static Teacher getTeacherById(const string& teacher_id); // Pobranie nauczyciela po ID

};

#endif // TEACHER_H
