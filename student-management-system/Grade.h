#ifndef GRADE_H
#define GRADE_H

#include <string>
#include <vector>
#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Grade {
public:
    static const string filePath; // Œcie¿ka do pliku JSON z ocenami

    string id;         // ID oceny
    double grade;      // Wartoœæ oceny
    string subject;    // Przedmiot
    string comment;    // Komentarz do oceny
    string date;       // Data wystawienia oceny

    // Konstruktor
    Grade(const string& id, double grade, const string& subject, const string& comment, const string& date)
        : id(id), grade(grade), subject(subject), comment(comment), date(date) {}

    // Destruktor
    ~Grade() {}

    // Operacje na pliku
    static vector<Grade> loadGradesFromFile(); // Wczytanie ocen z pliku JSON
    static void saveGradesToFile(const vector<Grade>& grades); // Zapisanie ocen do pliku JSON
};

#endif // GRADE_H
