#include "bcrypt.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{

    string password = "top_secret";

    string hash = bcrypt::generateHash(password);

    cout << "Hash: " << hash << endl;


	while (true) {
		string input;
		cout << "Enter password to hash: ";
		cin >> input;

		if (input == "end") {
			break;
		}

		string hashedInput = bcrypt::generateHash(input);
		cout << "Hashed Input: " << hashedInput << endl;

	}
    return 0;
}