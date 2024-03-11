/* README
 *
 * [1] "Factory Method" design pattern:
 * As the name suggests it uses methods or member functions to initialize an
 * object.
 * */


#include <iostream>
#include <cmath>

#define USE_MATH_DEFINES //M_PI_4
using namespace std;


class Point {
private:
    Point(float x, float y): x(x), y(y) { }
    float x, y;
public:

    static Point CreateCartesian(float x, float y) {
        return {x, y};
        //OR
        //return Point{x, y};
    }

    static Point CreatePolar(float rho, float theta) {
        return {rho * cos(theta), rho * sin(theta)};
    }

    void show() {
        cout << "x:" << x << " y:" << y << endl; 
    }

    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "x:" << p.x << " y:" << p.y;
        return os;
    }

};

int main() {

    Point p1 = Point::CreateCartesian(10, 5);
    p1.show();

    auto p2 = Point::CreatePolar(5, M_PI_4);
    cout << p2 << endl;

    return 0;
}
