#include "Grade.h"
#include <fstream>

const string Grade::filePath = "grades.json";

vector<Grade> Grade::loadGradesFromFile() {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << filePath << endl;
        return {};
    }

    json gradesData;
    file >> gradesData;

    vector<Grade> gradesList;
    for (const auto& gradeJson : gradesData) {
        Grade grade(
            gradeJson["id"].get<string>(),
            gradeJson["grade"].get<double>(),
            gradeJson["subject"].get<string>(),
            gradeJson["comment"].get<string>(),
            gradeJson["date"].get<string>()
        );
        gradesList.push_back(grade);
    }

    return gradesList;
}

void Grade::saveGradesToFile(const vector<Grade>& grades) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << filePath << endl;
        return;
    }

    json gradesData;
    for (const auto& grade : grades) {
        gradesData.push_back({
            {"id", grade.id},
            {"grade", grade.grade},
            {"subject", grade.subject},
            {"comment", grade.comment},
            {"date", grade.date}
            });
    }

    file << gradesData.dump(4); // Zapisanie w formacie JSON z wciêciami
}
