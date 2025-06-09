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
    static const string filePath; 

    string id;         
    double grade;    
    string subject;  
    string comment;   
    string date;    

    Grade(const string& id, double grade, const string& subject, const string& comment, const string& date)
        : id(id), grade(grade), subject(subject), comment(comment), date(date) {}

    ~Grade() {}

    static vector<Grade> loadGradesFromFile(); 
    static void saveGradesToFile(const vector<Grade>& grades);
};

#endif
