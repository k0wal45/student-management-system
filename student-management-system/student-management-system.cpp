#include "bcrypt.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{

    string password = "top_secret";

    string hash = bcrypt::generateHash(password);

    cout << "Hash: " << hash << std::endl;


	while (true) {
		string input;
		cout << "Enter password to verify: ";
		cin >> input;

		if (bcrypt::validatePassword(input, hash)) {
			cout << "Password is correct!" << endl;

			break;
		}
		else {
			cout << "Incorrect password." << endl;
		}
	}
    return 0;
}