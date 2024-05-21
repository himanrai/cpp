/*README
Using lambdas as an alternative for std::bind

Although std::bind provides a nice, terse syntax for creating function objects
that bind or reorder arguments of existing functions, it comes with a cost:
it makes the job of the compiler much more difficult, and it’s harder to
optimize. It’s implemented on the library level, and it uses many complex
template metaprogramming techniques to achieve its goal.

An alternative to using std::bind is to use lambdas for partial function
application. Lambdas are a core-language feature, which means the compiler
can optimize them more easily. The syntax will be a a bit more verbose, but
it’ll allow the compiler more freedom to optimize the code.

std::bind(greater, _1, value);

The equivalent lamdba code will be (arg is like _1):

[value] (int arg) {
    return greater(arg, value)
}
*/

#include <iostream>
#include <algorithm>
#include <functional>   // std::bind
#include <vector>

/*[1]
*/
void driver1()
{
    std::cout << "----- driver1():\n";

    //auto bound = std::bind(std::greater<double>(), 6, 42);
    auto bound = []() {
            return std::greater<double>()(6, 42);
        };
    bool is_6_greater_than_42 = bound();
    std::cout << std::boolalpha;
    std::cout << is_6_greater_than_42 << std::endl; // false
}

/*[2]
*/
void driver2()
{
    //using namespace std::placeholders; //adds visibility of _1, _2, _3,...

    std::cout << "----- driver2():\n";

    //auto is_greater_than = std::bind(std::greater<double>(), _1, _2);
    auto is_greater_than = [](double value1, double value2) {
            return std::greater<>()(value1, value2);
        };

    //auto is_x_greater_than_42 = std::bind(std::greater<double>(), _1, 42);
    auto is_x_greater_than_42 = [](int value1) {
            return std::greater<>()(value1, 42);
        };

    //auto is_42_greater_than_x = std::bind(std::greater<double>(), 42, _1);
    auto is_42_greater_than_x = [](int value1) {
            return std::greater<>()(42, value1);
        };

    std::cout << is_greater_than(6, 42) << std::endl; // returns false
    std::cout << is_x_greater_than_42(6) << std::endl; // returns false
    std::cout << is_42_greater_than_x(6) << std::endl; // returns true

    return;
}

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
    //using namespace std::placeholders; //adds visibility of _1, _2, _3,...

    std::cout << "----- driver3():\n";

    std::vector<Person> people {
        {"Joe",    "Dell",    20,   Person::male},
        {"Maria",  "Watson",  30,   Person::female},
        {"Tina",   "Wu",      32,   Person::female},
        {"Peter",  "Smith",   28,   Person::male},
        {"Ella",   "Yun",     25,   Person::female},
        {"Shaen",  "Martin",  36,   Person::male},
    };

    /*std::for_each(people.cbegin(), people.cend(),
            std::bind(print_person, _1, std::ref(std::cout),
                      Person::whole_name));*/

    //lambda equivalent to above std::bind()
    std::for_each(people.cbegin(), people.cend(),
            [](const Person& person) {
                print_person(person, std::ref(std::cout),
                      Person::whole_name);
            });

    /*
    std::ofstream file("test");
    std::for_each(people.cbegin(), people.cend(),
            std::bind(print_person, _1, std::ref(file),
                      Person::name_only));

    //lambda equivalent to above std::bind()
    std::for_each(people.cbegin(), people.cend(),
            [&file](const Person& person) {
                print_person(person, file,
                      Person::name_only);
            });
    */

    return;
}

void driver4()
{
    std::cout << "----- driver4():\n";

    std::vector<int> scores{39, 54, 86, 20, 14, 52, 30};

    //ascending order (std::sort used std::less<>() by default)
    //std::sort(scores.begin(), scores.end());

    //descending order
    //std::sort(scores.begin(), scores.end(), std::greater<>());

    //ascending order
    std::sort(scores.begin(), scores.end(), 
            [](double value1, double value2) {
                //arguments swapped
                return std::greater<>()(value2, value1);
            });

    for (auto score: scores) {
        std::cout << score << " ";
    }
    std::cout << std::endl;
}

int main() {
    driver1();
    driver2();
    driver3();
    driver4();

    return 0;
}