#ifndef STUDENTS_H
#define STUDENTS_H

#include <string>
#include <vector>
#include <iostream>
#include "Grade.h" // Dodano, aby uzyskaæ dostêp do klasy Grade
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Students {
public:
    static const string filePath; // Œcie¿ka do pliku JSON z danymi studentów

    string id;         // ID studenta
    string first_name; // Imiê
    string last_name;  // Nazwisko
    string student_id; // Numer studenta
    string email;      // Email
    string major;      // Kierunek studiów
    int year;          // Rok studiów
    string group;      // Grupa studencka
    vector<string> grades; // Lista ID ocen przypisanych do studenta

    // Konstruktor
    Students(const string& id, const string& first_name, const string& last_name,
        const string& student_id, const string& email, const string& major,
        int year, const string& group, const vector<string>& grades);

    // Destruktor
    ~Students();

    // Metody
    void addGrade(const string& grade_id);       // Dodanie oceny do studenta
    void removeGrade(const string& grade_id);    // Usuniêcie oceny ze studenta
    void listGrades() const;                     // Wyœwietlenie ocen studenta
    void displayStudentInfo() const;             // Wyœwietlenie informacji o studencie
    Grade getGrade(const string& grade_id) const; // Pobranie obiektu Grade na podstawie ID
    static void addStudent(const Students& newStudent); // Dodanie studenta
    static void removeStudent(const string& student_id); // Usuniêcie studenta
    static Students getStudentById(const string& student_id);

    // Operacje na plikach
    static vector<Students> loadStudentsFromFile(); // Wczytanie studentów z pliku JSON
    static void saveStudentsToFile(const vector<Students>& students); // Zapisanie studentów do pliku JSON
};

#endif // STUDENTS_H
