/* README
Using std::bind to bind values to specific function arguments

In 02_partial_func_app.cpp, we have seen the implementation of
bind2nd() function that converts binary functions into unary ones.

- Pre-C++11, the standard library provided two functions:
std::bind1st and std::bind2nd

- It provides a way to turn a binary function into a unary one by binding its
first or second argument to a specific value — much like bind2nd (the one we
implemented in 02_partial_func_app.cpp).

- In C++11, these two functions were deprecated (they were removed in C++17)
in favor of a much more general one called std::bind.

- The std::bind function isn’t limited to only binary functions and can work
with functions that have an arbitrary number of arguments. It also doesn’t
limit the user regarding which arguments can be bound; you can bind any number
of arguments, in any order, while leaving the rest unbound.
*/

#include <iostream>
#include <algorithm>
#include <functional>   // std::bind
#include <vector>


/*[1]
Let’s start with a basic example of using std::bind: binding all arguments of
a function to specific values.

The first argument of std::bind is the function whose arguments you want to
bind to specific values, and the other arguments are the values the function
arguments will be bound to. 
*/
void driver1()
{
    std::cout << "----- driver1():\n";

    auto bound = std::bind(std::greater<double>(), 6, 42);
    bool is_6_greater_than_42 = bound();
    std::cout << std::boolalpha;
    std::cout << is_6_greater_than_42 << std::endl; // false
}

/*[2]
Now, let’s see how you can leave one of the arguments unbound while binding
the other one. You can’t just define one value and skip the other, because
std::bind wouldn’t know which argument you want to bind — the first or the
second. Therefore, std::bind introduces the concept of *placeholders*.

If you want to bind an argument, you pass a value for it to std::bind as you
did in the preceding example. But if you want to say that an argument should
remain unbound, you have to pass a placeholder (_1, _2, and so on) instead.
*/
void driver2()
{
    using namespace std::placeholders; //adds visibility of _1, _2, _3,...

    std::cout << "----- driver2():\n";

    auto is_greater_than = std::bind(std::greater<double>(), _1, _2);
    auto is_x_greater_than_42 = std::bind(std::greater<double>(), _1, 42);
    auto is_42_greater_than_x = std::bind(std::greater<double>(), 42, _1);

    std::cout << is_greater_than(6, 42) << std::endl; // returns false
    std::cout << is_x_greater_than_42(6) << std::endl; // returns false
    std::cout << is_42_greater_than_x(6) << std::endl; // returns true

    return;
}

/*[3] driver3(): Using std::bind on functions with more arguments

You started with a function that needs three arguments - the person,
the output stream, and the output format - and used it to create two
new functions. Each takes a person as its argument but behaves differently.

One writes the full name of a person to the standard output, and the other
writes the person’s first name to the specified file. And you did this without
writing the new functions by hand; you used the existing function, bound a
couple of its arguments, and got a unary function that you can use in the
std::for_each algorithm as the result.
*/
#include "../ch-02/person.h"
void print_person(const Person& person,
                    std::ostream& out,
                    Person::output_format_e format)
{
    if (format == Person::name_only) {
        out << person.first_name() << '\n';
    } else if (format == Person::whole_name) {
        out << person.full_name() << '\n';
    }
}

void driver3()
{
    using namespace std::placeholders; //adds visibility of _1, _2, _3,...

    std::cout << "----- driver3():\n";

    std::vector<Person> people {
        {"Joe",    "Dell",    20,   Person::male},
        {"Maria",  "Watson",  30,   Person::female},
        {"Tina",   "Wu",      32,   Person::female},
        {"Peter",  "Smith",   28,   Person::male},
        {"Ella",   "Yun",     25,   Person::female},
        {"Shaen",  "Martin",  36,   Person::male},
    };

    /* std::bind(): createing a unary function that prints the name of a
       person to the standard output.
    */
    std::for_each(people.cbegin(), people.cend(),
            std::bind(print_person, _1, std::ref(std::cout),
                      Person::whole_name));

    /* std::bind(): createing a unary function that prints the name of a
       person to a file.
    */
    /*
    std::ofstream file("test");
    std::for_each(people.cbegin(), people.cend(),
            std::bind(print_person, _1, std::ref(file),
                      Person::name_only));
    */

    return;
}

/*[4] driver4():

So far, you’ve preferred non-member functions or static member functions
to proper class member functions, because class member functions aren’t
considered function objects — they don’t support the function call syntax.

This limitation is artificial. Member functions are essentially the same as
ordinary ones, except they have an implicit first argument this that points
to the instance of the class the member function was called on.

You have a print_person() function that takes three arguments: person_t, an
output stream, and a format. You can replace this function with a member
function inside the Person class (refer "../ch-02/person.h").

In essence, there’s no difference between the print_person and Person::print
functions except that C++ doesn’t allow you to call the latter by using the
usual call syntax. The Person::print() member function also has three
arguments — an implicit argument 'this' that refers to a person, and two
explicit arguments out and format — and it does the same thing as
print_person().

Fortunately, std::bind can bind arguments of any callable, and it treats
print_person() and Person::print the same. To use member function, you need
to replace print_person() with a pointer to the Person::print member function.
*/
void driver4()
{
    using namespace std::placeholders; //adds visibility of _1, _2, _3,...

    std::cout << "----- driver4():\n";

    std::vector<Person> people {
        {"Joe",    "Dell",    20,   Person::male},
        {"Maria",  "Watson",  30,   Person::female},
        {"Tina",   "Wu",      32,   Person::female},
        {"Peter",  "Smith",   28,   Person::male},
        {"Ella",   "Yun",     25,   Person::female},
        {"Shaen",  "Martin",  36,   Person::male},
    };

    /* std::bind(): creating a unary function object from a member
       function pointer.
    */
    std::for_each(people.cbegin(), people.cend(),
            std::bind(&Person::print, _1, std::ref(std::cout),
                      Person::whole_name));

    return;
}


int main() {
    driver1();
    driver2();
    driver3();
    driver4();

    return 0;
}
