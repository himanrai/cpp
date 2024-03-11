/* README
 *
 * "Inner-Factory" design pattern:
 *  - It is just a flavor of "Factory" design pattern (1_factrory.cpp).
 *  - If you see 1_factrory.cpp, class Point has no public method using which
 *    client can infer: "how to create a Point". And just looking at the code,
 *    there is no obvious connection between Point and PointFactory which
 *    client can quickly see and understand that using PointFactory, a Point
 *    can be created. Through documentation it can be conveyed, however looking
 *    at the code it is not obvious.
 *  - To make the Point creation logic avaialble within the Point class, 
 *    we will move PointFactory class inside the Point class so that by looking
 *    at the Point class, clients can get to know about PointFactory and using
 *    it they can create a Point.
 *  - We are naming it "Inner-Factory" because we are moving PointFactory
 *    inside Point.
 *  - In GoF (Gang of Four) there is no design pattern called "Factory"
 *    or "Inner-Factory". Gof has "Factory Method" and "Abstarct Factory"
 *    design patterns.
 * */


#include <iostream>
#include <cmath>

#define USE_MATH_DEFINES //M_PI_4
using namespace std;

//comment below line to enable code for private PointFactory
#define POINTFACTORY_PUBLIC


#if defined (POINTFACTORY_PUBLIC)
/* We can have inner PointFactory as public.
 * */
class Point {
private:
    Point(float x, float y): x(x), y(y) { }
    float x, y;

public:

    class PointFactory {
    public:
        static Point CreateCartesian(float x, float y) {
            return {x, y}; 
        }

        static Point CreatePolar(float rho, float theta) {
            return {rho * cos(theta), rho * sin(theta)};
        }   
    };

public:
    void show() {
        cout << "x:" << x << " y:" << y << endl; 
    }

    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "x:" << p.x << " y:" << p.y;
        return os;
    }

};


#else

/* OR We can have PointFactory as private and a static member of
 * PointFactory as public.
 * */
class Point {
private:
    Point(float x, float y): x(x), y(y) { } 
    float x, y;

private:
    class PointFactory {
    public:
        static Point CreateCartesian(float x, float y) {
            return {x, y}; 
        }

        static Point CreatePolar(float rho, float theta) {
            return {rho * cos(theta), rho * sin(theta)};
        }
    };

public:
    void show() {
        cout << "x:" << x << " y:" << y << endl; 
    }

    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "x:" << p.x << " y:" << p.y;
        return os; 
    }

    static PointFactory factory;

};

#endif

int main() {

#if defined (POINTFACTORY_PUBLIC)
    Point p1 = Point::PointFactory::CreateCartesian(10, 5);
    p1.show();

    auto p2 = Point::PointFactory::CreatePolar(5, M_PI_4);
    cout << p2 << endl;
#else
    Point p1 = Point::factory.CreateCartesian(10, 5);
    p1.show();

    auto p2 = Point::factory.CreatePolar(5, M_PI_4);
    cout << p2 << endl;
#endif

    return 0;
}
