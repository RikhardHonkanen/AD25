/**
 * @file exercise-29.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of class member templates
 *
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

template <typename T>
class A
{
    template <typename V>
    struct B
    {
        void f(T x, V y);
    };

public:
    void print(const T &val)
    {
        B<int> b;
        b.f(val, 25);
        std::cout << "print" << std::endl;
    }

    template <typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    void func(void)
    {
        std::cout << "func" << std::endl;
    }
};

template <typename T>
template <typename V>
void A<T>::B<V>::f(T x, V y)
{
    std::cout << x << ", " << y << std::endl;
}

int main(void)
{
    A<std::string> p;
    p.print("Hello");
    p.func<int>();
    // p.func<int *>(); /* Error: int * is not an arithmetic type */

    return 0;
}