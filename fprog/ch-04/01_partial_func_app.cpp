/*
README:
Partial function application:

The concept of creating a new function object from an existing one by fixing
one or more of its arguments to a specific value is called *partial function
application*. The word partial in this case means you provide some, but not
all, arguments needed to calculate the result of the function.

Let us explore it below.
*/

#include <iostream>
#include <algorithm> // std::partition
#include <vector>

/*[1] Function:
If you need to check an object that is older than a certain predefined age
limit, then conceptually you would have a function that takes two arguments:
an object (for example, a person or a car) and a value that will be compared
against the age of that object. This function returns true if the age of the
object is greater than that value.
*/
template<typename T>
bool older_than_func(T&& obj, int limit) {
    return std::forward<T>(obj).age() > limit;
}

/*[2] Functor (ch-03/07_generic_functor.cpp)
But you cannot use the above function with std::count_if algorithm, because it
expects you to give it a unary predicate, you have to create a function object
that stores the age limit inside itself, and use that value when it gets the
object whose age needs to be checked.

=> The notion that you don’t need to pass in at once all the arguments a
function requires is something we’ll explore a bit more in this ch-04..
*/
class older_than {
public:
    older_than(int limit): m_limit(limit) {}

    template<typename T>
    bool operator()(T&& obj) {
        return std::forward<T>(obj).age() > m_limit;
    }

private:
    int m_limit;
};


/* [3]
Let’s look at a simpler example. Instead of checking whether a person’s age
is greater than a specified value, you’ll take the general greater-than
operator (a function that takes two arguments) and bind its second
argument to a fixed value, thus creating a unary function.

Here we are *converting binary function into unary one*.
*/
//binary function:
bool greater_than_binary_func(int arg, int value) {
    return arg > value;
}

//converted to unary one:
class greater_than {
public:
    greater_than(int value): m_value(value) {}

    bool operator()(int arg) {
        return arg > m_value;
    }

private:
    int m_value;
};

void driver1() {
    
    greater_than greater_than_42(42);
    std::cout << std::boolalpha;
    std::cout << greater_than_42(1) << std::endl; // false
    std::cout << greater_than_42(50) << std::endl; // true

    std::vector<int> vec {1, 4, 20, 5, 6, 3, 7, 10};
    auto bound = std::partition(vec.begin(), vec.end(), greater_than(6));

    std::cout << "Elements greater than 6: ";
    for (auto itr = vec.begin(); itr != bound; itr++) {
        std::cout << " " << *itr;
    }
    std::cout << std::endl;

    std::cout << "Elements less than 6: ";
    for (auto itr = bound; itr != vec.end(); itr++) {
        std::cout << " " << *itr;
    }
    std::cout << std::endl;

    std::cout << "All elements after partition:";
    for (auto elem: vec) {
        std::cout << " " << elem;
    }
    std::cout << std::endl;

    return;
}

int main() {
    driver1();

    return 0;
}