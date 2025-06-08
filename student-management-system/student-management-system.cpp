#include <iostream>
#include "auth.h"

int main() {
    std::string login, password;

    std::cout << "Podaj login: ";
    std::cin >> login;

    std::cout << "Podaj hasło: ";
    std::cin >> password;

    std::string role = Auth::AuthUser(login, password);

    if (!role.empty()) {
        std::cout << "Zalogowano pomyślnie! Rola: " << role << std::endl;
    }
    else {
        std::cout << "Autoryzacja nieudana." << std::endl;
    }

    return 0;
}
