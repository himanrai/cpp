#include <iostream>
#include <cmath>

using namespace std;

enum class PointType {
    cartesian,
    polar
};

/* [1]
 * We have a class Point and we would like to initialize it using
 * cartesian and polar coordinates.
 * */
class Point {
private:
    float x, y;
public:
    /* [2] Init cartesian coordinates*/
    Point(float x, float y): x(x), y(y) { }

    /* [3] Init polar coordinates
     * [4] ISSUE-1: We cannot have another constructor with same type of parameters.
     * */
    #if 0
    Point(float rho, float theta) {
        x = rho * cos(theta); 
        y = rho * sin(theta); 
    }
    #endif

    /* [5] To solve ISSUE-1 we have to have a constructor with coordinate type.
     * [6] ISSUE-2: Here its not clear what a and b are, and for this comments
     *     need to be added:
     *     !param a: this is either x or rho
     *     !param b: this is either y or theta
     * */
    Point(float a, float b, PointType type = PointType::cartesian) {
        if (type == PointType::cartesian) {
            x = a;
            y = b;
        } else if (type == PointType::polar) {
            x = a * cos(b); 
            y = a * sin(b); 
        }
    }

    /* [7] To have a better solution we should have methods like:
     *     - PointFromCartesian()
     *     - PointFromPolar()
     * [8] This is what the "factory method" is all about. Making sure
     *     that the methods a class provides is sensible, understandable and
     *     immediately consumable by the clients. By looking at the methods
     *     client can know how the construction of object can happen.
     * */


    void show() {
        cout << "x:" << x << " y:" << y << endl; 
    }

};

int main() {

    Point p1(10, 5, PointType::cartesian);
    p1.show();

    return 0;
}
