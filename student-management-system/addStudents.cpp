#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

void printStudents(const string& filePath) {
    ifstream file(filePath);

    // Sprawdz, czy plik zostal otwarty
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << filePath << endl;
        return;
    }

    try {
        // Wczytaj dane JSON z pliku
        json studentsData;
        file >> studentsData;

        // Iteruj po studentach i drukuj ich dane
        cout << "Lista studentow:\n";
        for (const auto& student : studentsData) {
            cout << "Dane studenta: " << student.dump() << endl;

            // Sprawdz, czy klucze istnieja
            if (student.contains("id")) {
                cout << "ID: " << student["id"] << endl;
            }
            else {
                cerr << "Nieprawidlowy format danych studenta.\n";
            }

			if (student.contains("name")) {
				cout << "Imie: " << student["name"] << endl;
			}
			else {
				cerr << "Nieprawidlowy format danych studenta.\n";
			}

			if (student.contains("age")) {
				cout << "Wiek: " << student["age"] << endl;
			}
			else {
				cerr << "Nieprawidlowy format danych studenta.\n";
			}

			if (student.contains("major")) {
				cout << "Kierunek: " << student["major"] << endl;
			}
			else {
				cerr << "Nieprawidlowy format danych studenta.\n";
			}

			if (student.contains("email")) {
				cout << "Email: " << student["email"] << endl;
			}
			else {
				cerr << "Nieprawidlowy format danych studenta.\n";
			}



            cout << "--------------------------\n";
        }
    }
    catch (const json::exception& e) {
        cerr << "Blad podczas parsowania JSON: " << e.what() << endl;
    }
}
