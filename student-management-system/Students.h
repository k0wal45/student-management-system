#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Students
{
public:
    string id;
    string first_name;
    string last_name;
    string student_id;
    string email;
    string major;
    int year;
    string group;
    vector<string> grades;

    // Konstruktor z parametrami
    Students(const string& id, const string& first_name, const string& last_name,
        const string& student_id, const string& email, const string& major,
        int year, const string& group, const vector<string>& grades);

    // Destruktor
    ~Students();

    // Metody
    void addGrade(const string& grade_id);
    void removeGrade(const string& grade_id);
    void listGrades() const;
    void displayStudentInfo() const;

    // Statyczne metody do obs³ugi JSON
    static vector<Students> loadStudentsFromFile();
    static void saveStudentsToFile(const vector<Students>& students);

    // Statyczne pole: œcie¿ka do pliku
    static const string filePath;
};
