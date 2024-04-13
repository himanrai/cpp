#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "person.h"

std::string first_name(const Person &person) {
    return person.first_name();
}

std::string full_name(const Person &person) {
    return person.full_name();
}

bool is_female(const Person &person) {
    return person.gender() == Person::female;
}

std::vector<std::string>
filter_females(const std::vector<Person> &people) {

    std::vector<Person> females;

    //Filtering females from a collection of people based
    //on the is_female predicate
    std::copy_if(people.cbegin(), people.cend(),
        std::back_inserter(females), is_female);
    
    std::vector<std::string> female_names(females.size());

    /*After filtering, we need a construct that takes a group of people and
      returns their names. Similar to filtering, this construct can’t know
      in advance what information you want to collect from the original items.
      You might want to get a value of a specific attribute (the name, in this
      example), combine multiple attributes (perhaps fetch and concatenate
      both the first name and the surname), or do something more complex
      (get a list of children for each person).
    */
    std::transform(females.cbegin(), females.cend(),
        female_names.begin(), first_name);
    /*std::transform(females.cbegin(), females.cend(),
        female_names.begin(), full_name);*/

    return female_names;
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

    auto females_name = filter_females(people);

    std::cout << "Females full name:" << std::endl;
    for (const auto &name: females_name) {
        std::cout << name << std::endl;
    }
}