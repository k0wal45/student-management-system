#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <vector>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Auth {
public:
    static string AuthUser(const string& email, const string& password);
};

#endif
