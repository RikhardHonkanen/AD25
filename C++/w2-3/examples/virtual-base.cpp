/**
 * @file exercise-29.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a virtual base class
 *
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

#define VIRTUAL 0 // Change it to 1 to make A as a virtual base class

class A
{
    int a;

public:
    A(int _a) : a{_a} { std::cout << "A Constructed" << std::endl; }
};

#if VIRTUAL
class B : virtual public A
#else
class B : public A
#endif
{
    int b;

public:
    B(int _a, int _b) : A{_a}, b{_b} { std::cout << "B Constructed" << std::endl; }
};

#if VIRTUAL
class C : virtual public A
#else
class C : public A
#endif
{
    int c;

public:
    C(int _a, int _c) : A{_a}, c{_c} { std::cout << "C Constructed" << std::endl; }
};

class D : public B, public C
{
    int d;

public:
#if VIRTUAL
    D(int _a, int _b, int _c, int _d) : A{_a}, B{_a, _b}, C{_a, _c}, d{_d}
#else
    D(int _a, int _b, int _c, int _d) : B{_a, _b}, C{_a, _c}, d{_d}
#endif
    {
        std::cout << "D Constructed" << std::endl;
    }
};

int main(void)
{
    D d{1, 2, 3, 4};

    std::cout << "-----------------------" << std::endl;

    C c{10, 20};

    return 0;
}