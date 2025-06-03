#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Students
{
public: 
	string id;
	string name;
	int age;



	Students();

	~Students();

	void addStudent(const string& name, int age);
	void removeStudent(const string& name);
	void listStudents() const;
};

