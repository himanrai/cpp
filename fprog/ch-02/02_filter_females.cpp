/* README:
[1] std::copy_if(), then std::transform()
[2] Composability problems of STL algorithms

[1] Refer filter_females()
Both filter and transform are common programming patterns that many
programmers implement repeatedly in projects they work on.
- std::copy_if(): To filter Persons by their gender
- std::transform(): To get name of Persons

[2] Refer filter_females_v2()

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


/*
Composability problems of STL algorithms
========================================
- The solution from filter_females() is valid and will work correctly for any
  type of input collection that can be iterated on, from vectors and lists to
  sets, hash maps, and trees. It also shows the exact intent of the program:
  to copy all females from the input collection and then get their names.
- Unfortunately, filter_females() approach isn’t as efficient or simple as a
  handwritten loop in filter_females_v2().
- The STL-based implementation makes unnecessary copies of people (which might
  be an expensive operation or could even be disabled if the copy constructor
  was deleted or private), and it creates an additional vector that isn’t
  needed. You could try to compensate for these problems by using references
  or pointers instead of copies, or by creating smart iterators that skip all
  persons who aren’t females, and so on. But the need to do this extra work
  is a clear indication that STL loses in this case; the handwritten loop is
  better and requires less effort.
*/
std::vector<std::string>
filter_females_v2(const std::vector<Person> &people) {
   
    std::vector<std::string> female_names;

    for (const auto& person : people) {
        if (is_female(person)) {
            female_names.push_back(first_name(person));
        }
    }

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