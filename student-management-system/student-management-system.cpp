#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    const char* word = getenv("WORD");

    if (word) {
        cout << "The value of WORD is: " << word << endl;
    }
    else {
        cout << "Environment variable WORD is not set." << endl;
    }

    cout << "pls work this time too" << endl;

    return 0;
}
