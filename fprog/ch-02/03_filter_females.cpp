/* README: *erase-remove* idiom

If we can alter the sequence then we can use *erase-remove* idiom
to filter the Persons by their geneder.

- vector::erase()
- std::remove_if()
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "person.h"

bool is_female(const Person &person) {
    return person.gender() == Person::female;
}

bool is_male(const Person &person) {
    return person.gender() == Person::male;
}

void 
filter_females(std::vector<Person> &people) {
    people.erase(
        std::remove_if(people.begin(), people.end(), is_male),
        people.end()
    );

    return;
}

int main() {

    std::vector<Person> people {
        {"Joe",    "Dell",    20,   Person::male},
        {"Maria",  "Watson",  30,   Person::female},
        {"Tina",   "Wu",      32,   Person::female},
        {"Peter",  "Smith",   28,   Person::male},
        {"Ella",   "Yun",     25,   Person::female},
        {"Shaen",  "Martin",  36,   Person::male},
    };

    filter_females(people);

    for (const Person &person: people) {
        std::cout << person.first_name() << std::endl;
    }

    return 0;
}