#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <vector>
#include <iostream>
#include "json.hpp" 
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
    vector<string> grades; 
    vector<string> exams; 

    static const string filePath; 

    Teacher(const string& id, const string& first_name, const string& last_name,
        const string& subject, const string& email, const vector<string>& grades, const vector<string>& exams);
    ~Teacher();

    void addGrade(const string& student_id, const Grade& grade);
    void removeGrade(const string& grade_id);
    void addExam(const string& student_id, const Exam& exam);
    void removeExam(const string& exam_id);
    void displayTeacherInfo() const;

    static vector<Teacher> loadTeachersFromFile();
    static void saveTeachersToFile(const vector<Teacher>& teachers);

    static void addTeacher(const Teacher& newTeacher, const string& password); 
    static void removeTeacher(const string& teacher_id); 
    static Teacher getTeacherById(const string& teacher_id); 

};

#endif 
