/*README

Reference book:
Functional Programming in cpp

Perfect forwarding will be explained as:
[1] Perfectly forward the result
[2] Perfect forwarding for arguments
[3] Example code#1
[4] Example code#2
*/

/*
[1] Perfectly forward the result

decltype(auto) is useful when you’re writing generic functions that forward
the result of another function without modifying it. In this case, you don’t
know in advance what function will be passed to you, and you can’t know
whether you should pass its result back to the caller as a value or as a
reference. If you pass it as a reference, it might return a reference to a
temporary value that will produce undefined behavior. And if you pass it as
a value, it might make an unnecessary copy of the result. Copying will have
performance penalties and is sometimes semantically wrong — the caller may
expect a reference to an existing object.

=> If you want to perfectly forward the result (to return the result returned
to you without any modifications), you can use decltype(auto) as the
specification of the return type:

template <typename Object, typename Function>
decltype(auto) call_on_object(Object&& object, Function function)
{
    return function(std::forward<Object>(object));
}

In this code snippet, you have a simple function template that takes an
object, and a function that should be invoked on that object. You’re perfectly
forwarding the object argument to the function; and by using decltype(auto) as
the return type, you’re perfectly forwarding the result of the function back
to your caller.

 
=> If you don’t have a compiler that supports automatic return type deduction,
you need to use decltype to achieve the same effect. Otherwise, you could
have written decltype(auto) as mentioned above.

auto call_on_object(Object&& object, Function function)
    -> decltype(function(std::forward<Object>(object)))
{
    return function(std::forward<Object>(object));
}

*/

/*
[2] Perfect forwarding for arguments:

You sometimes need to write a function that wraps in another function.
The only thing it should do is call the wrapped function with some arguments
modified, added, or removed. In that case, you have the problem of how to
pass the arguments from the wrapper to the function you need to call.

That means the user is passing you a function you know nothing about.
You don’t know how it expects you to pass it the argument.

[2.1]
The first option is to make your call_on_object function accept the object
argument by *value* and pass it on to the wrapped function. This will lead
to problems if the wrapped function accepts a reference to the object,
because it needs to change that object. The change won’t be visible outside
the call_on_object function because it’ll be performed on the local copy of
the object that exists only in the call_on_object function:

template <typename Object, typename Function>
decltype(auto) call_on_object(Object object, Function function)
{
    return function(object);
}

[2.2]
The second option is to pass the object by reference. This would make the
changes to the object visible to the caller of your function. But you’ll
have a problem if function doesn’t actually change the object but accepts
the argument as a const-reference. The caller won’t be able to invoke
call_on_object on a constant object (see Example below) or a temporary value.
The idea to accept the object as an ordinary reference still isn’t right.
And you can’t make that reference const, because the function may want to
change the original object.

template <typename Object, typename Function>
decltype(auto) call_on_object(Object &object, Function function)
{
    return function(object);
}

Example:
int func1(int& arg) {
    return arg;
}
int main() {
    const int i = 1;
    //Error: binding reference of type ‘int&’ to ‘const int’ discards qualifiers
    int val = func1(i);
    return 0;
}

Solutions:
[2.3]
Some pre-C++11 libraries approached this problem by creating overloads for
both const and non-const references. This isn’t practical because the number
of overloads grows exponentially with the number of arguments that need to
be forwarded.

[2.4]
This is solved in C++11 with forwarding references (formerly known as
universal references 2). A forwarding reference is written as a double
reference on a templated type. In the following code, the fwd argument
is a forwarding reference to type T, whereas value isn’t (it’s a normal
rvalue reference):

template <typename T>
void f(T&& fwd, int&& value) { ... }

The forwarding reference allows you to accept both const and non-const
objects, and temporaries. Now you only need to pass that argument, and pass
it in the same value category as you’ve received it, which is exactly what
std::forward does.

*/

#include <iostream>
#include <string>
#include <utility>


//[3] Example code#1

void overloaded_function(std::string& param) {
    std::cout << "std::string& version" << std::endl;
}
void overloaded_function(std::string&& param) {
    std::cout << "std::string&& version" << std::endl;
}

template<typename T>
void pass_through(T&& param) {
    //overloaded_function(param);
    overloaded_function(std::forward<T>(param));
}

void main1() {
    std::string pes;
    pass_through(pes);
    pass_through(std::move(pes));
}

/*
Output of main1():
std::string& version
std::string&& version
*/

/*
Conceptual implementation of std::forward

template<typename T>
T&& forward(T&& param)     //in namespace std
{
    if (is_lvalue_reference<T>::value) { // if T indicates lvalue
        return param;                    // do nothing
    } else {                             // else
        return move(param);              // cast to rvalue
    }
}
*/

//[4] Example code#2

#include <stdio.h>

using std::puts;

struct S{};

void func(S& s) { puts("func(S& s)"); }
void func(S&& s) { puts("func(S&& s)"); }

template<typename T>
void wrap(T&& param) {
    //func(param);
    func(std::forward<T>(param));
}

void main2() {
    S s;
    func(s);   //lvalue
    func(S()); //rvalue

    wrap(s);   //lvalue
    wrap(S()); //rvalue

}

/*
Output of main2():
func(S& s)
func(S&& s)
func(S& s)
func(S&& s)
*/

int main() {
    main1();
    std::cout << "------" << std::endl;
    main2();
}
