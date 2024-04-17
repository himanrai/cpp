/* README:
[1] Receiving functions as arguments

[1]
- In the 02_filter_females.cpp, you wanted to get the names of all females
  in a collection. Suppose you have a collection of people and often need
  to get the names that satisfy a certain predicate, but you don’t want to
  limit yourself to a predefined predicate such as is_female. You want to
  support any predicate that takes a person_t. The user might want to separate
  the people based on their age, hair color, marital status, and so on.

- Refer names_for()
  Creating a function that you could call multiple times would be useful.
  The function would need to accept a vector of people and a predicate function
  to be used for filtering, and it would return a vector of strings containing
  the names of people satisfying that predicate.

    template <typename FilterFunction>
    std::vector<std::string> names_for(
            const std::vector<person_t>& people,
            FilterFunction filter)

*/

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

bool is_not_female(const Person &person) {
    return !is_female(person);
}

template <typename FilterFunction>
std::vector<std::string> names_for(
        const std::vector<Person>& people,
        FilterFunction filter)
{
    std::vector<std::string> result;
    for (const Person& person : people) {
        if (filter(person)) {
            result.push_back(first_name(person));
        }
    }
    return result;
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

    auto females_name = names_for(people, is_female);
    std::cout << "\nFemales full name:" << std::endl;
    for (const auto &name: females_name) {
        std::cout << name << std::endl;
    }

    auto males_name = names_for(people, is_not_female);
    std::cout << "\nMales full name:" << std::endl;
    for (const auto &name: males_name) {
        std::cout << name << std::endl;
    }
}