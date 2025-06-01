#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>

using namespace std;

int main() {
    // Sprawdzenie zmiennej środowiskowej WORD
    const char* word = getenv("WORD");
    if (word) {
        cout << "The value of WORD is: " << word << endl;
    }
    else {
        cout << "Environment variable WORD is not set." << endl;
    }

    // Inicjalizacja drivera MongoDB
    mongocxx::instance instance{};
    mongocxx::client client{ mongocxx::uri{} };

    // Połączenie z bazą i kolekcją
    mongocxx::database db = client["test"];              // baza 'test'
    mongocxx::collection coll = db["example"];           // kolekcja 'example'

    // Wstawienie dokumentu
    bsoncxx::builder::stream::document document{};
    document << "word" << (word ? word : "undefined");

    coll.insert_one(document.view());

    cout << "Inserted a document into MongoDB." << endl;

    return 0;
}