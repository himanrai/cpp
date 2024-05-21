/*README:
- STL containers have value_type.
- we will see the usage of value_type in template.

Example:
template<typename C, typename T = typename C::value_type>
*/

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <algorithm>


void driver1( )
{
    std::cout << "----- driver(1):" << std::endl;

    std::vector<int>::value_type AnInt;
    AnInt = 44;
    std::cout << AnInt << std::endl;
}

/*In driver1(), it is pretty easy to know that you need an int, 
where it gets complicated is generic programming. For example, if I wanted to
write a generic sum() function, I would need it to know what kind of container
to iterate and what type its elements are, so I would need to have something
like this:*/

template<typename Container>
typename Container::value_type sum1(const Container& cont)
{
    typename Container::value_type total = 0;
    for (const auto& e : cont)
        total += e;
    return total;
}

//OR
template<typename Container, typename T = typename Container::value_type>
T sum2(const Container& cont)
{
    T total = 0;
    for (const auto& e : cont)
        total += e;
    return total;
}

//OR
template<typename C, typename T = typename C::value_type>
T sum3(const C& collection)
{
    T total = 0;
    for (const auto& c : collection)
        total += c;
    return total;
}


void driver2() {
    std::cout << "----- driver(2):" << std::endl;

    std::vector<int> container1{1, 2, 3, 4, 5};
    std::list<double> container2{1.1, 2.1, 3.1, 4.1, 5.1};

    std::cout << sum1(container1) << std::endl;
    std::cout << sum1(container2) << std::endl;

    //std::cout << sum2(container1) << std::endl;
    //std::cout << sum2(container2) << std::endl;

    //std::cout << sum3(container1) << std::endl;
    //std::cout << sum3(container2) << std::endl;
}

int main() {
    driver1();
    driver2();
}