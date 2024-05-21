/* README
Partial function application:
A generic way to *convert binary functions into unary ones*.
*/

#include <iostream>
#include <algorithm> // std::partition
#include <vector>

/*[1]
Let’s try to make the 01_partial_func_app.cpp implementation more generic.
You want to create a function object that can wrap any binary function that
the user passes to it, and bind one of its arguments. For consistency’s sake,
you’re going to bind the second argument, as the greater_than class did.

NOTE: The standard library used to contain a function called std::bind2nd,
which implemented the same concept as the one presented below.

- The function object needs to be able to store a binary function and one of
its arguments. Because you don’t know in advance the types of the function and
the second argument, you need to create a class templated on these two types.

- You don’t know the type of the first argument in advance, so you need to make
the call operator a template as well. The implementation is straightforward:
you call the function that you’ve stored in the m_function member and forward
it the argument passed to the call operator as the first argument, along with
the value stored in the m_second_arg member as the second argument.
*/
template<typename Function, typename SecondArgType>
class partial_app_bind2nd_arg {
private:
    Function m_function;
    SecondArgType m_second_arg;

public:
    partial_app_bind2nd_arg(Function func, SecondArgType second_arg):
        m_function(func), m_second_arg(second_arg) {}

    template<typename FirstArgType>
    auto operator()(FirstArgType&& first_arg) const
        -> decltype(m_function(
            std::forward<FirstArgType>(first_arg),
            m_second_arg))
    {
        return m_function(
            std::forward<FirstArgType>(first_arg),
            m_second_arg);
    }

    /* If you don’t have a compiler that supports automatic return type
       deduction, you need to use decltype ('to declare the return type'
       as used above) to achieve the same effect. Otherwise, you could
       have written decltype(auto) like below.
       Use either the above or below one.
    */
    /*template<typename FirstArgType>
    decltype(auto) operator()(FirstArgType&& first_arg) const
    {
        return m_function(
            std::forward<FirstArgType>(first_arg),
            m_second_arg
        );
    }*/

};

/*
[2]
- Now that you’ve defined the complete class, you need one more thing to make
it useful. If you want to use it directly from your code, you must specify the
template argument types explicitly when creating an instance of this class.
This would be ugly, and in some cases impossible (for example, you don’t know
the type of the lambda).

NOTE: The requirement of class template instantiation for the template
arguments to be explicitly defined has been removed in C++17. But because
this standard hasn’t yet been widely accepted, the examples in ch-04 don’t
rely on this feature.

- To have the compiler deduce the types automatically, create a function
template whose job is only to make an instance of this class. Because
template-argument deduction works when calling functions, you don’t need
to specify the types when calling it.

- The function calls the constructor for the class you defined previously
and forwards its arguments to the constructor. It’s mostly boilerplate that
the language forces you to write in order to have a way to instantiate a
class template without having to explicitly specify the template arguments.
*/

template <typename Function, typename SecondArgType>
partial_app_bind2nd_arg<Function, SecondArgType>
bind2nd(Function&& function, SecondArgType&& second_arg) {

    return partial_app_bind2nd_arg<Function, SecondArgType>(
                std::forward<Function>(function),
                std::forward<SecondArgType>(second_arg));
}

void driver1() {
    
    auto greater_than_42 = bind2nd(std::greater<int>(), 42);

    std::cout << std::boolalpha;
    std::cout << greater_than_42(1) << std::endl; // false
    std::cout << greater_than_42(50) << std::endl; // true

    std::vector<int> vec {1, 4, 20, 5, 6, 3, 7, 10};
    auto bound = std::partition(vec.begin(), vec.end(),
                                 bind2nd(std::greater<int>(), 6));

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

/*
To show that it’s truly more general, driver2() uses the newly created bind2nd
function for multiplication rather than the greater-than relation.
*/
void driver2() {
    #define PI 3.14

    std::vector<double> degrees = {0, 30, 45, 60};
    std::vector<double> radians(degrees.size());
    
    std::transform(degrees.cbegin(), degrees.cend(),
        radians.begin(),
        bind2nd(std::multiplies<double>(), PI / 180));

    std::cout << "Radians:";
    for (auto radian: radians) {
        std::cout << " " << radian;
    }
    std::cout << std::endl;

    return;
}


int main() {
    driver1();
    driver2();

    return 0;
}

/*
NOTE:

- Pre-C++11, the standard library provided two functions:
std::bind1st and std::bind2nd

- It provides a way to turn a binary function into a unary one by binding its
first or second argument to a specific value—much like bind2nd (the one we
implemented above).

- In C++11, these two functions were deprecated (they were removed in C++17)
in favor of a much more general one called std::bind.

- The std::bind function isn’t limited to only binary functions and can work
with functions that have an arbitrary number of arguments. It also doesn’t
limit the user regarding which arguments can be bound; you can bind any number
of arguments, in any order, while leaving the rest unbound.
*/