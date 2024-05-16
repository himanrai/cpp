/*README:
The following example demonstrates calling
- function pointers,
- function references, and
- objects convertible to function pointers (conversion function)
*/

#include <iostream>

int ask() { return 42; }
typedef decltype(ask)* function_ptr;

class convertible_to_function_ptr {
public:
    operator function_ptr() const //conversion function
    {
        return ask;
    }
};

int main(int argc, char* argv[])
{
    auto ask_ptr = &ask;
    std::cout << ask_ptr() << '\n';

    auto& ask_ref = ask;
    std::cout << ask_ref() << '\n';

    convertible_to_function_ptr ask_wrapper;
    std::cout << ask_wrapper() << '\n';
}

/*
This example demonstrates that you can create a function pointer (ask_ptr)
that points to an ordinary function, and a function reference (ask_ref)
that references the same function, and that you can call them as if they
were functions themselves—with the usual function call syntax. It also
demonstrates that you can create an object that’s convertible to a function
pointer and call that object as if it were a normal function without any
complications.
*/