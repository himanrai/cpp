/* README:
 * - Drink Factory is going to be the concrete implementation of Abstarct
 *   Factory.
 * - It is actually a facilitator for using all the different factories
 *   that we have made and having them in some sort of map and exposing them
 *   to whoever actaully making a drink.
 * */

/* - #pragma once is a non-standard but widely supported preprocessor
 *   directive designed to cause the current header file to be included
 *   only once in a single compilation.
 * */
#pragma once

#include <map>
#include <functional>
#include "HotDrinkFactory.hpp"


using namespace std;

/* - This class is going to help us construct drinks on the basis of different
 *   factories.
 * */
class DrinkFactory {
private:
    /* key: name of the drink
     * value: Factory that is capable of producing that drink
     * */
    //OBJECT ORIENTED APPROACH
    map<string, unique_ptr<HotDrinkFactory>> hot_factories;
public:
    DrinkFactory() {
        hot_factories["tea"] = make_unique<TeaFactory>();
        hot_factories["coffee"] = make_unique<CoffeeFactory>();
    };

    unique_ptr<HotDrink> make_drink(const string& name) {
         auto drink = hot_factories[name]->make();
         drink->prepare(150);
         return drink;
    };
};

class DrinkWithVolumeFactory {
private:
    //FUNCTIONAL_APPROACH
    map<string, function<unique_ptr<HotDrink>()>> factories;
public:
    DrinkWithVolumeFactory() {
        factories["tea"] = [] {
                auto tea = make_unique<Tea>();
                tea->prepare(100);
                return tea;
            };
        factories["coffee"] = [] {
                auto coffee = make_unique<Coffee>();
                coffee->prepare(150);
                return coffee;
            };
    }

    unique_ptr<HotDrink> make_drink(const string& name) {
        return factories[name]();
    }
};
