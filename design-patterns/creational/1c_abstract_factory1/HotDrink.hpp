/* - #pragma once is a non-standard but widely supported preprocessor
 *   directive designed to cause the current header file to be included
 *   only once in a single compilation.
 * */
#pragma once


#include <iostream>

using namespace std;

/*[1] It is a Product*/
struct HotDrink {
    virtual ~HotDrink() = default;
    virtual void prepare(int volume) = 0;
};

/* [2] Here we have family of different Products:
 * - Tea
 * - Coffee
 * */

struct Tea: HotDrink {
    virtual void prepare(int volume) override {
        cout << "prepare Tea of volume: " << volume << endl;
    }
};


struct Coffee: HotDrink {
    virtual void prepare(int volume) override {
        cout << "prepare Coffee of volume: " << volume << endl;
    }
};

