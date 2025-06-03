#include "printStudents.h"
#include "json.hpp"
#include "Students.h"

int main() {


    string path = "students.json"; // Upewnij się, że ścieżka jest poprawna

    cout << "System zarzadzania studentami\n";
    cout << "1. Pokaz studentow\n";
    cout << "2. Wyjdz\n";
    cout << "Wybierz opcje: ";
    int option;
    cin >> option;
    cin.ignore();

    if (option == 1) {
        printStudents(path);
    }
    else if (option == 2) {
        cout << "Wyjscie z programu.\n";
    }
    else {
        cout << "Nieprawidlowa opcja.\n";
    }

    return 0;
}
