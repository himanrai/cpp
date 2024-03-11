/* - #pragma once is a non-standard but widely supported preprocessor
 *   directive designed to cause the current header file to be included
 *   only once in a single compilation.
 * */
#pragma once


#include <memory>
#include "HotDrink.hpp"

/* [1] HotDrinkFactory is an "Abstract Factory" */
struct HotDrinkFactory {

    virtual unique_ptr<HotDrink> make() const = 0;
};


/* [2]
 * - We have family of Products (HotDrink.hpp)
 * - Here we have cooresponding Factories
 * */
struct TeaFactory: HotDrinkFactory {

    virtual unique_ptr<HotDrink> make() const override {
        return make_unique<Tea>();
    }
};

struct CoffeeFactory: HotDrinkFactory {

    virtual unique_ptr<HotDrink> make() const override {
        return make_unique<Coffee>();
    }
};
