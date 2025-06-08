#include "auth.h"
#include <fstream>
#include <iostream>
#include "bcrypt.h"
#include "json.hpp"

using json = nlohmann::json;

std::string Auth::AuthUser(const std::string& login, const std::string& password) {
    try {
        if (login.empty() || password.empty()) {
            std::cerr << "Login lub has�o s� puste." << std::endl;
            return ""; // Autoryzacja nieudana
        }

        // Wczytanie danych u�ytkownik�w z pliku JSON
        std::ifstream file("users.json");
        if (!file.is_open()) {
            std::cerr << "Nie mo�na otworzy� pliku users.json" << std::endl;
            return ""; // Autoryzacja nieudana
        }

        json users;
        file >> users; // Pr�ba parsowania JSON-a
        file.close();

        // Szukanie u�ytkownika po loginie
        for (const auto& user : users) {
            if (user["login"] == login) {
                std::string storedHash = user["password"];
                if (bcrypt::validatePassword(password, storedHash)) {
                    return user["role"]; // Zwr�cenie roli u�ytkownika
                }
                else {
                    std::cerr << "Niepoprawne has�o." << std::endl;
                    return ""; // Has�o niepoprawne
                }
            }
        }

        std::cerr << "U�ytkownik nie znaleziony." << std::endl;
        return ""; // U�ytkownik nie znaleziony
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "B��d parsowania JSON-a: " << e.what() << std::endl;
        return "";
    }
    catch (const std::exception& e) {
        std::cerr << "Wyst�pi� b��d: " << e.what() << std::endl;
        return "";
    }
}
