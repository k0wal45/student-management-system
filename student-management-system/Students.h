#ifndef STUDENTS_H
#define STUDENTS_H

#include <string>
#include <vector>
#include <iostream>
#include "Grade.h" 
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Students {
public:
    static const string filePath; 

    string id;         
    string first_name; 
    string last_name;  
    string student_id; 
    string email;      
    string major;      
    int year;          
    string group;      
    vector<string> grades; 

    Students(const string& id, const string& first_name, const string& last_name,
        const string& student_id, const string& email, const string& major,
        int year, const string& group, const vector<string>& grades);

    ~Students();

    void addGrade(const string& grade_id);       
    void removeGrade(const string& grade_id);    
    void listGrades() const;                     
    void displayStudentInfo() const;             
    Grade getGrade(const string& grade_id) const; 
    static void addStudent(const Students& newStudent, const string& password); 
    static void removeStudent(const string& student_id); 
    static Students getStudentById(const string& student_id);

    static vector<Students> loadStudentsFromFile(); 
    static void saveStudentsToFile(const vector<Students>& students); 
};

#endif 
