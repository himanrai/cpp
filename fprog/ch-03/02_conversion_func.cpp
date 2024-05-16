/* README:
- bool convesrion function:
  operator bool() const;

- explicit conversion operator:
  explicit operator bool() const;
*/

#include <iostream>

//Example-1
template <typename T>
struct S {
    operator bool() const {   // conversion function
        return true;
    }
};

void func(S<int>& s) {
    // The compiler converts s to the bool type implicitly through
    // the conversion function. This conversion might be *intended*.
    if (s) {
        std::cout << "object s is true" << std::endl;
    }
}

void bar(S<int>& s1, S<float>& s2) {
    // The compiler converts both s1 and s2 to the bool type implicitly
    // through the conversion function. This conversion might be *unintended*.
    std::cout << s1+s2 << std::endl; 
  
    // The compiler converts both s1 and s2 to the bool type implicitly
    // through the conversion function and compares results. 
    // This conversion might be *unintended*.
    if (s1 == s2) {
        std::cout << "object s1 == s2" << std::endl;
    }  
}

//Example-2
struct T {
  explicit operator bool() {   //explicit bool conversion operator
      return true;
  }
};

int main() {
    S<int> s1;
    S<float> s2;
    func(s1);
    bar(s1, s2);

    T t1;
    bool t2 = true;
  
    // The compiler converts t1 to the bool type through 
    // the explicit bool conversion operator implicitly.
    std::cout << (t1 && t2) << std::endl; 

    return 0;
}

/*
NOTES:

- To inhibit unintended implicit conversions from being applied, you can
  define an explicit conversion operator by qualifying the conversion function
  in Example-1 with the explicit function specifier:

  explicit operator bool() const;

- If you compile the same code as Example-1 but with the explicit conversion
  operator, the compiler issues error messages for the following statements:

  // Error: The call does not match any parameter list for "operator+".
  std::cout << p1+p2 << std::endl; 
 
  // Error: The call does not match any parameter list for "operator==".
  if(p1==p2)

- If you intend to apply the conversion through the explicit conversion
  operator, you must call the explicit conversion operator explicitly as
  in the following statements, and then you can get the same results as
  Example-1.

  std::cout << bool(p1)+bool(p2) << std::endl; 

  if(bool(p1)==bool(p2))

- In contexts where a Boolean value is expected, such as when &&, ||, or
  the conditional operator is used, or when the condition expression of
  an if statement is evaluated, an explicit bool conversion operator can
  be implicitly invoked. So when you compile Example-1 with the previous
  explicit conversion operator, the compiler also converts s in the func
  function to the bool type through the explicit bool conversion operator
  implicitly.
*/