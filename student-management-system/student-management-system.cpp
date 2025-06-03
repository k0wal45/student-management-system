#include <iostream>
#include "Students.h"

using namespace std;

int main() {
    try {
        // Wczytanie studentów z pliku JSON
        vector<Students> students = Students::loadStudentsFromFile();

        // Wyświetlenie liczby studentów
        cout << "Liczba studentow: " << students.size() << endl;

        // Iteracja po studentach i wyświetlenie ich informacji oraz ocen
        for (const auto& student : students) {
            cout << "\nInformacje o studencie:" << endl;
            student.displayStudentInfo(); // Wyświetlenie informacji o studencie i jego ocenach
        }
    }
    catch (const exception& e) {
        cerr << "Wystapil blad: " << e.what() << endl;
    }

    return 0;
}
