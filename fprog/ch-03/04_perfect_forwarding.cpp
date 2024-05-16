/* README

Reference:
https://www.modernescpp.com/index.php/perfect-forwarding/

[1] Rvalues 
[2] Lvalues 
[3] Perfect forwarding
*/

/* [1] Rvalues are
- temporary objects.
- objects without names.
- objects which have no address.

[1.1] Example of Rvalues:

int five= 5;
std::string a= std::string("Rvalue");
std::string b= std::string("R") +  std::string("value");
std::string c= a + b;
std::string d= std::move(b);

Description:
Rvalues are on the right side of an assignment. The value 5 and the
constructor call std::string(“Rvalue”) are rvalues because you can
neither determine the address of the value 5 nor has the created
string object a name. The same holds for the addition of the rvalues
in the expression std::string("R") +  std::string("value").

The addition of the two strings a + b is interesting. Both strings
are lvalues, but the addition creates a temporary object. A particular
use case is std::move(b). The new C++11 function converts the lvalue b
into an rvalue reference.

*/

/* [2] Lvalues
- Lvalues have a name and an address.


*/

/* [3] Perfect forwarding

[3.1] A function should have the following characteristics:
- It can take an arbitrary number of arguments
- Can accept lvalues and rvalues as an argument
- Forwards its arguments identical to the underlying constructor

See below codes:
perfectForwarding1()
perfectForwarding2()
perfectForwarding3()
perfectForwarding4()
*/

#include <iostream>

/* perfectForwarding1()
For efficiency reasons, the function template should take its arguments
by reference.

If we compile the perfectForwarding1(), it will give a compiler error.
The reason is that the rvalue can not be bound to a non-constant lvalue reference.
*/

template<typename T, typename Arg>
T create1(Arg& arg) {
    return T(arg);
}

void perfectForwarding1() {
    std::cout << "\nperfectForwarding1()" << std::endl;

    int five = 5;
    int value1 = create1<int>(five);
    //int value1 = create1<int, int>(five); //same as above
    std::cout << "value1: " << value1 << std::endl;

    //Error: rvalue can not be bound to a non-constant lvalue reference
    //int value2 = create1<int>(5);
    //std::cout << "value2: " << value2 << std::endl;
}

/*
We have two ways to solve the issue.

1. Change the non-constant lvalue reference (line 6) in a constant lvalue
   reference. You can bind an rvalue to a constant lvalue reference. But
   that is not perfect because the function argument is constant, and I
   can not change it.
2. Overload the function template for a constant and non-const lvalue
   reference. That is easy. That is the right way to go.
   perfectForwarding2()
*/

template<typename T, typename Arg>
T create2(Arg& arg) {
    return T(arg);
}

template<typename T, typename Arg>
T create2(const Arg& arg) {
    return T(arg);
}

void perfectForwarding2() {
    std::cout << "\nperfectForwarding2()" << std::endl;

    int five = 5;
    int value1 = create2<int>(five);
    std::cout << "value1: " << value1 << std::endl;

    int value2 = create2<int>(5);
    std::cout << "value2: " << value2 << std::endl;
}

/*
perfectForwarding2() has two conceptual issues.

1. To support n different arguments, I have to overload  2^n +1 variations
   of the function template create. 2^n +1 because the function create without
   an argument is part of the perfect factory method.
2. If the T constructor (inside create2(const Arg& arg)) needs an rvalue,
   it will no longer work.
*/


/* perfectForwarding3():
using std::forward, the solution looks promising.

- universal reference (Arg&& arg) is powerful and can bind lvalues or rvalues.
- IMPORTANT: To achieve perfect forwarding, you must combine a universal
  reference with std::forward.
- std::forward<Arg>(arg) returns the underlying type because arg is a universal
  reference. Therefore, an rvalue remains an rvalue.
*/

template<typename T, typename Arg>
T create3(Arg&& arg) {
    return T(std::forward<Arg>(arg));
}

void perfectForwarding3() {
    std::cout << "\nperfectForwarding3()" << std::endl;

    int five = 5;
    int value1 = create3<int>(five);
    std::cout << "value1: " << value1 << std::endl;

    int value2 = create3<int>(5);
    std::cout << "value2: " << value2 << std::endl;
}

/*
So the pattern of Perfect forwarding is:

template<class T>
void wrapper(T&& a){
    func(std::forward<T>(a)); 
}
*/

/*
perfectForwarding4():
Perfect forwarding with Variadic Templates 
*/

#include <string>
#include <utility>

template<typename T, typename ... Arg>
T create4(Arg&& ... arg) {
    return T(std::forward<Arg>(arg)...);
}

struct MyStruct{
  MyStruct(int i,double d,std::string s){}
};

void perfectForwarding4() {
    std::cout << "\nperfectForwarding4()" << std::endl;

    int five = 5;
    int value1 = create4<int>(five);
    std::cout << "value1: " << value1 << std::endl;

    std::string str{"Lvalue"};
    std::string str2 = create4<std::string>(str);
    std::cout << "str2: " << str2 << std::endl;

    int value2 = create4<int>(5);
    std::cout << "value2: " << value2 << std::endl;

    std::string str3= create4<std::string>(std::string("Rvalue"));
    std::cout << "str3: " << str3 << std::endl;

    //converts the lvalue b into an rvalue reference.
    std::string str4= create4<std::string>(std::move(str3));
    std::cout << "str4: " << str4 << std::endl;
  
    // Arbitrary number of arguments
    double doub= create4<double>();
    std::cout << "doub: " << doub << std::endl;
  
    MyStruct myStr= create4<MyStruct>(2011,3.14,str4);

}

int main() {
    perfectForwarding1();
    perfectForwarding2();
    perfectForwarding3();
    perfectForwarding4();
}


