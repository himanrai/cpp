/*README:
Conversion functions:
- A conversion operator is a special kind of member function that converts a
  value of a class type to a value of some other type. A conversion function
  typically has the general form:
      operator typeName() const;
  where typeName represents a type. 

  Note the following points:
   - The conversion function must be a class method.
   - The conversion function must not specify a return type.
   - The conversion function must have no arguments.

  For example, a function to convert to type double would have this prototype:
      operator double() const;

- Conversion operators can be defined for any type (other than void) that can
  be a function return type.
- Conversions to an array or a function type are not permitted. 
- Conversions to pointer types (both data and function pointers) and to
  reference types are allowed.
*/

/*
Example:
Let us add conversion functions that convert Stonewt objects to type
int and to type double. It requires adding the following prototypes to
the class declaration:
    operator int();
    operator double();
*/

#include <iostream>

class Stonewt
{
private:
    enum {Lbs_per_stn = 14}; // pounds per stone
    int stone; // whole stones
    double pds_left; // fractional pounds
    double pounds; // entire weight in pounds

public:
    Stonewt(double lbs); // construct from double pounds
    Stonewt(int stn, double lbs); // construct from stone, lbs
    Stonewt(); // default constructor
    ~Stonewt();

    void show_lbs() const; // show weight in pounds format
    void show_stn() const; // show weight in stone format

    // conversion functions
    operator int() const;
    operator double() const;
};

using std::cout;

// construct Stonewt object from double value
Stonewt::Stonewt(double lbs)
{
    stone = int (lbs) / Lbs_per_stn; // integer division
    pds_left = int (lbs) % Lbs_per_stn + lbs - int(lbs);
    pounds = lbs;
}

// construct Stonewt object from stone, double values
Stonewt::Stonewt(int stn, double lbs)
{
    stone = stn;
    pds_left = lbs;
    pounds = stn * Lbs_per_stn +lbs;
}

Stonewt::Stonewt() // default constructor, wt = 0
{
    stone = pounds = pds_left = 0;
}
Stonewt::~Stonewt() // destructor
{
}

// show weight in stones
void Stonewt::show_stn() const
{
    cout << stone << " stone, " << pds_left << " pounds\n";
}

// show weight in pounds
void Stonewt::show_lbs() const
{
    cout << pounds << " pounds\n";
}

// conversion functions
Stonewt::operator int() const
{
    return int (pounds + 0.5);
}
Stonewt::operator double()const
{
    return pounds;
}

int main()
{
    using std::cout;
    Stonewt poppins(9,2.8); // 9 stone, 2.8 pounds
    double p_wt = poppins; // implicit conversion
    cout << "Convert to double => ";
    cout << "Poppins: " << p_wt << " pounds.\n";
    cout << "Convert to int => ";
    cout << "Poppins: " << int (poppins) << " pounds.\n";
    return 0;
}

/*
NOTES#1:

With cout above code uses "int (poppins)". Suppose that, instead, it omitted
the explicit type cast:
cout << "Poppins: " << poppins << " pounds.\n";

Would the program use an implicit conversion, as in the following statement?
double p_wt = poppins;

The answer is no. In the p_wt example, the context indicates that poppins
should be converted to type double. But in the cout example, nothing indicates
whether the conversion should be to int or to double. Facing this lack of
information, the compiler would complain that you were using an ambiguous
conversion. Nothing in the statement indicates what type to use.

Interestingly, if the class defined only the double conversion function, the
compiler would accept the statement.That’s because with only one conversion
possible, there is no ambiguity.

*/

/*
NOTES#2:

The problem with providing functions that make automatic, implicit conversions
is that they may make conversions when you don’t expect them. Suppose, for
example, that you happen to write the following code:

int ar[20];
...
Stonewt temp(14, 4);
...
int Temp = 1;
...
cout << ar[temp] << "!\n"; // used temp instead of Temp

Normally, you’d expect the compiler to catch a blunder such as using an object
instead of an integer as an array index. But the Stonewt class defines an
operator int(), so the Stonewt object temp is converted to the int 200 and be
used as an array index.

The moral is that often it’s best to use explicit conversions and exclude the
possibility of implicit conversions. In C++98, the keyword explicit doesn’t
work with conversion functions, but C++11 removes that limitation. So with
C++11, you can declare a conversion operator as explicit:

class Stonewt
{
...
    // conversion functions
    explicit operator int() const;
    explicit operator double() const;
};

With these declarations in place, you would use a type cast to invoke the operators.
*/