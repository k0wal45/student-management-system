#include "auth.h"
#include <fstream>
#include <iostream>
#include "bcrypt.h"
#include "json.hpp"

using json = nlohmann::json;

std::string Auth::AuthUser(const std::string& login, const std::string& password) {
    try {
        if (login.empty() || password.empty()) {
            std::cerr << "Login lub has³o s¹ puste." << std::endl;
            return ""; // Autoryzacja nieudana
        }

        // Wczytanie danych u¿ytkowników z pliku JSON
        std::ifstream file("users.json");
        if (!file.is_open()) {
            std::cerr << "Nie mo¿na otworzyæ pliku users.json" << std::endl;
            return ""; // Autoryzacja nieudana
        }

        json users;
        file >> users; // Próba parsowania JSON-a
        file.close();

        // Szukanie u¿ytkownika po loginie
        for (const auto& user : users) {
            if (user["login"] == login) {
                std::string storedHash = user["password"];
                if (bcrypt::validatePassword(password, storedHash)) {
                    return user["role"]; // Zwrócenie roli u¿ytkownika
                }
                else {
                    std::cerr << "Niepoprawne has³o." << std::endl;
                    return ""; // Has³o niepoprawne
                }
            }
        }

        std::cerr << "U¿ytkownik nie znaleziony." << std::endl;
        return ""; // U¿ytkownik nie znaleziony
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "B³¹d parsowania JSON-a: " << e.what() << std::endl;
        return "";
    }
    catch (const std::exception& e) {
        std::cerr << "Wyst¹pi³ b³¹d: " << e.what() << std::endl;
        return "";
    }
}
