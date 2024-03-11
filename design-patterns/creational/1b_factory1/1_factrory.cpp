/* README
 *
 * "Factory" design pattern:
 *  - In GoF (Gang of Four) there is no design pattern called "Factory".
 *    Gof has "Factory Method" and "Abstarct Factory" design patterns.
 * */

#include <iostream>
#include <cmath>

#define USE_MATH_DEFINES //M_PI_4
using namespace std;


class Point {
private:
    /* [2] - To solve [1] we made PointFactory a friend
     *     - But this violates OCP (open-closed principle)
     *     - As an alternative you, can have the constructor public
     *     - If you make constructor public then client has 2 ways to
     *       construct the Point: through constructor (only Cartesian)
     *       and through PointFactory.
     * */ 
    friend class PointFactory;

    Point(float x, float y): x(x), y(y) { }
    float x, y;
public:

    void show() {
        cout << "x:" << x << " y:" << y << endl; 
    }

    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "x:" << p.x << " y:" << p.y;
        return os;
    }

};

class PointFactory {
public:
    /* [1] private constructor of Point is inaccessible, so we will
     *     have to make PointFactory a friend of Point
     * */

    static Point CreateCartesian(float x, float y) {
        return {x, y};
    }

    static Point CreatePolar(float rho, float theta) {
        return {rho * cos(theta), rho * sin(theta)};
    }
};

int main() {

    Point p1 = PointFactory::CreateCartesian(10, 5);
    p1.show();

    auto p2 = PointFactory::CreatePolar(5, M_PI_4);
    cout << p2 << endl;

    return 0;
}
