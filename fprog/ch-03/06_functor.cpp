#include <iostream>
#include <vector>
#include <algorithm>
#include "../ch-02/person.h"

/*README

Call operator overloading
=========================
Instead of creating types that can be implicitly converted to function
pointers (03_func_pointer.cpp), C++ provides a much nicer way to create new
types that behave like functions:
by creating classes and overloading their call operators.

Unlike other operators, a call operator can have an arbitrary number of
arguments, and the arguments can have any type, so you can create a function
object of any signature you like.

The syntax for overriding the call operator is as simple as defining a member
function—just with a special name of operator(). You specify the return type
and all the
arguments the function needs:

class function_object {
public:
    return_type operator()(arguments) const
    {
        ...
    }
};

These function objects have one advantage compared to the ordinary functions:
each instance can have its own state, whether mutable or not. The state is
used to customize the behavior of the function without the caller having to
specify it.

*/

/*
Say you have a list of people, You want to allow the user to count the number
of people who are older than a specified age. If the age limit is fixed, you
could create an ordinary function to check whether the person was older than
that predefined value.
*/
bool older_than_42(const Person& person) {
    return person.age() > 42;
}

/*
The above solution isn’t scalable because you’d need to create separate
functions for all age limits you needed or use an error-prone hack, such
as saving the age limit you were currently testing against in a global
variable.

Instead, it’s much wiser to create a proper function object that keeps
the age limit as its inner state. This will allow you to write the
implementation of the predicate only once and instantiate it several
times for different age limits:
*/
class older_than {
public:
    older_than(int limit)
        : m_limit(limit) { }

    bool operator()(const Person& person) const {
        return person.age() > m_limit;
    }
private:
    int m_limit;
};

/*
You can now define multiple variables of this type and use them as functions:

older_than older_than_42(42);
older_than older_than_14(14);

if (older_than_42(person)) {
    std::cout << person.name() << " is more than 42 years old\n";
} else if (older_than_14(person)) {
    std::cout << person.name() << " is more than 14 years old\n";
} else {
    std::cout << person.name() << " is 14 years old, or younger\n";
}
*/

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
        older_than_42);
    std::cout << "Number of people older than 42 are: " << count << std::endl;

    count = std::count_if(people.cbegin(), people.cend(),
        older_than(42));
    std::cout << "Number of people older than 42 are: " << count << std::endl;
    count = std::count_if(people.cbegin(), people.cend(),
        older_than(16));
    std::cout << "Number of people older than 16 are: " << count << std::endl;

}