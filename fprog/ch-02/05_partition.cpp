/* README

- std::partion()
- std::stable_partion()
Both algorithms take a collection and a predicate. They reorder the items in
the original collection so that the items that satisfy the specified predicate
are separated from those that don’t. The items that satisfy the predicate
are moved to the beginning of the collection, and the items that don’t are
moved to the back. The algorithm returns an iterator to the first element in
the second group (to the first element that doesn’t satisfy the predicate).
The difference between these two algorithms is that std::stable_partition
retains the ordering between elements from the same group.

Females first:
Partitioning a group of people based on a predicate that checks whether
a person is female. As the result, all females are moved to the start
of the collection.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "person.h"

bool is_female(const Person &person) {
    return person.gender() == Person::female;
}

bool is_not_female(const Person &person) {
    return !is_female(person);
}

void
female_first_v1(std::vector<Person> people) {

    std::partition(people.begin(), people.end(),
        is_female);

    std::cout << "\nfemale_first_v1():" << std::endl;
    for (const Person &person: people) {
        std::cout << person.first_name() << " : " << person.gender() << std::endl;
    }
}

void
female_first_v2(std::vector<Person> people) {

    //Retains the ordering between elements.
    std::stable_partition(people.begin(), people.end(),
        is_female);

    std::cout << "\nfemale_first_v2():" << std::endl;
    for (const Person &person: people) {
        std::cout << person.first_name() << " : " << person.gender() << std::endl;
    }
}


void
female_first_v3(std::vector<Person> &people) {

    std::vector<Person> persons(people.size());

    const auto last = std::copy_if(people.begin(), people.end(),
        persons.begin(), is_female);
    
    std::copy_if(people.begin(), people.end(),
        last, is_not_female);

    std::cout << "\nfemale_first_v2():" << std::endl;
    for (const Person &person: persons) {
        std::cout << person.first_name() << " : " << person.gender() << std::endl;
    }
}

/*
If you need to move selected items to the middle of the list, you can split
the list into the part above the destination point and the part below.
*/
void
female_middle(std::vector<Person> people) {

    std::stable_partition(people.begin(), people.begin()+3,
        is_not_female);
    std::stable_partition(people.begin()+3, people.end(),
        is_female);

    std::cout << "\nfemale_middle():" << std::endl;
    for (const Person &person: people) {
        std::cout << person.first_name() << " : " << person.gender() << std::endl;
    }
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

    female_first_v1(people);
    female_first_v2(people);
    female_first_v3(people);
    female_middle(people);

    return 0;
}