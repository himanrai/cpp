#include <iostream>
#include <memory>
#include "HotDrinkFactory.hpp"
#include "DrinkFactory.hpp"


/* In a typical case where Abstarct Factory is not being used,
 * you will write a function which will retrun different type
 * of Products (drinks in our case) depeneding upon the drink
 * type passed to the function. 
 * */
unique_ptr<HotDrink> make_drink(string type) {
    unique_ptr<HotDrink> drink;

    if (type == "tea") {
        drink = make_unique<Tea>();
        drink->prepare(100);
    } else if (type == "coffee") {
        drink = make_unique<Coffee>();
        drink->prepare(50);
    }
    return drink;
}


int main() {

    //auto d = make_drink("tea");

    /*It will keep track of "kind of factories" that we have*/
    DrinkFactory df;
    df.make_drink("tea");

    return 0;
}
