/*README
Currying: a different way to look at functions

- Currying is named after Haskell Curry, a mathematician and logician
- currying often looks much like partial function application to the untrained
  eye.
- Say you’re working in a programming language that doesn’t let you create
  functions that have more than a single argument.
- Instead of creating a function that takes two arguments and returns a single
  value, you can create a unary function that returns a second unary function.
  When this second function is called, this means you’ve received both needed
  arguments and can return the resulting value.

*/

#include <iostream>

/*[1]
Let’s see this idea in a simple example. You have a function called greater
that takes two values and checks whether the first one is greater than the
second.

On the other hand, you have its curried version, which can’t return a bool
value because it only knows the value of the first argument. It returns a
unary lambda that captures the value of that argument. The resulting lambda
will compare the captured value with its argument when it gets invoked.
*/

// greater : (double, double) → bool
bool greater(double first, double second) {
    return first > second;
}

// greater_curried : double → (double → bool)
auto greater_curried(double first) {
    return [first](double second) {
        return first > second;
    };
}

void driver1() {
    std::cout << "----- driver1():\n";

    // Invocation
    std::cout << std::boolalpha;
    std::cout << greater(2, 3) << std::endl;

    auto func = greater_curried(2);
    std::cout << func(3) << std::endl;

    std::cout << greater_curried(2)(3) << std::endl; 
}

/*[2] Example-2
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

/* You can call it like so:
print_person(person, std::cout, person_t::full_name);

You can convert this function to its curried version in the same way you did
for greater — by nesting enough lambda expressions to capture all the arguments
needed to execute print_person one by one.
*/
auto print_person_curried(const Person& person) {
    return [&](std::ostream& out) {
        return [&](Person::output_format_e format) {
            print_person(person, out, format);
        };
    };
}

//TODO: FIXME
#if 0
//Not sure why driver2() is giving compilation error.
void driver2() {
    std::cout << "----- driver2():\n";
    using std::cout;
    Person martha{"Martha",    "Li",    20,   Person::female};

    print_person_curried(martha)(cout)(Person::full_name);

    auto print_martha = print_person_curried(martha);
    print_martha(cout, Person::name_only);
}
#endif

/*[3]
Because writing code like print_person_curried() is tedious, you’re going to
use a helper function called make_curried from now on. This function can
convert any function into its curried version.

Even more, the resulting curried function will provide you with syntactic
sugar, which will allow you to specify more than one argument at a time
if needed. This is syntactic sugar; the curried function is still just a
unary function, as I said before, just more convenient to use.

NOTE You’ll see how to implement a function like this in chapter 11. At this
point, the implementation isn’t important; you just need it to do what you
want.

Before we move on to the possible uses of currying in C++, let’s first
demonstrate what you can do with the make_curried function and what the
usage syntax is like.
*/
#if 0
void driver3() {
    std::cout << "----- driver3():\n";
    using std::cout;
    Person martha{"Martha",    "Li",    20,   Person::female};

    auto print_person_cd = make_curried(print_person);

    print_person_cd(martha, cout, Person::full_name);
    print_person_cd(martha)(cout, Person::full_name);
    print_person_cd(martha, cout)(Person::full_name);
    print_person_cd(martha)(cout)(Person::full_name);

    auto print_martha = print_person_cd(martha);
    print_martha(cout, Person::name_only);

    auto print_martha_to_cout = print_person_cd(martha, cout);
    print_martha_to_cout(Person::name_only);
}
#endif

int main() {
    driver1();
    //driver2();
    //driver3();
    return 0;
}