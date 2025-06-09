#ifndef EXAM_H
#define EXAM_H

#include <string>
#include <vector>
#include <iostream>
#include "json.hpp" 

using namespace std;
using json = nlohmann::json;

class Exam {
public:

    string id;
    string date;
    string comment;
    string subject;
    string teacher_id;

    Exam(const string& id, const string& date, const string& comment, const string& subject, const string& teacher_id);
    ~Exam();

    static vector<Exam> loadExamsFromFile();
    static void saveExamsToFile(const vector<Exam>& exams);

    static void addExam(const Exam& newExam); 
    static void removeExam(const string& exam_id); 
    static Exam getExamById(const string& exam_id);

    void displayExamInfo() const;

    static const string filePath; 
};

#endif 
