#include <iostream>
#include <vector>
#include <algorithm>
#include "../ch-02/person.h"

/* README

In 06_functor.cpp, you created a function object that checks whether a person
is older than a predefined age limit. This solved the problem of needing to
create different functions for different age limits, but it’s still overly
restrictive: it accepts only Persons as input.

Quite a few types could include age information—from concrete things like cars
and pets to more abstract ones like software projects. If you wanted to count
the number of cars that were older than 5 years, you couldn’t use the preceding
function object because it’s defined only for Person.
*/


/*
The first valid approach you could take would be to turn the older_than class
into a class template. It would be templated on the type of the object whose
age you want to check.

Unfortunately, this approach forces you to specify the type of the object whose
age you want to check when you instantiate the function object. Although this
is sometimes useful, in most cases it’s tedious.
older_than<Person>(16)
older_than<Car>(5)
older_than<Project>(2)
*/
template<typename T>
class older_than {
public:
    older_than(int limit)
        : m_limit(limit) { }

    bool operator()(const T& object) const {
        return object.age() > m_limit;
    }
private:
    int m_limit;
};


/*
Instead of creating the class template, you can make the call operator a
template member function. This way, you won’t need to specify the type when
you instantiate the older_than function object, and the compiler will
automatically deduce the type of the argument when invoking the call operator.
*/
class older_than_v2 {
public:
    older_than_v2(int limit)
        : m_limit(limit) { }

    template<typename T>
    bool operator()(T&& object) const {
        return std::forward<T>(object).age() > m_limit;
    }
private:
    int m_limit;
};


int main() {
    std::vector<Person> people {
        {"Joe",    "Dell",    20,   Person::male},
        {"Maria",  "Watson",  30,   Person::female},
        {"Tina",   "Wu",      52,   Person::female},
        {"Peter",  "Smith",   28,   Person::male},
        {"Ella",   "Yun",     65,   Person::female},
        {"Shaen",  "Martin",  18,   Person::male},
    };

    int count;

    count = std::count_if(people.cbegin(), people.cend(),
                older_than<Person>(42));
    std::cout << "Number of people older than 42 are: " << count << std::endl;
    count = std::count_if(people.cbegin(), people.cend(),
                older_than<Person>(16));
    std::cout << "Number of people older than 16 are: " << count << std::endl;

    /*
    count = std::count_if(cars.cbegin(), cars.cend(),
                older_than<Car>(5));
    count = std::count_if(projects.cbegin(), projects.cend(),
                older_than<Project>(2));
    */

    older_than_v2 predicate(5);
    //std::cout << predicate(people[0]) << std::endl;
    count = std::count_if(people.cbegin(), people.cend(), predicate);
    std::cout << "Number of people older than 5 are: " << count << std::endl;
    //std::count_if(cars.cbegin(), cars.cend(), predicate);
    //std::count_if(projects.cbegin(), projects.cend(), predicate);

    return 0;

}