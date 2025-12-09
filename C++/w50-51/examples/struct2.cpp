#include <iostream>

struct Data
{
    static int a;
    static const int b{10};
    static constexpr int c{20};
    const volatile int d{30};
    mutable int e{40};
    const int f{50};
    int g{60};
};

struct Date
{
    uint32_t day : 5;
    uint32_t month : 4;
    uint32_t year : 23;
};

int Data::a;

int main(void)
{
    Data data{100, 200, 300, 400};

    std::cout << data.a << std::endl;
    std::cout << data.b << std::endl;
    std::cout << data.c << std::endl;
    std::cout << data.d << std::endl;
    std::cout << data.e << std::endl;
    std::cout << data.f << std::endl;
    std::cout << data.g << std::endl;

    Date today{18, 1, 2023};
    std::cout << today.year << "-" << today.month << "-" << today.day << std::endl;

    std::cout << sizeof(Date) << std::endl;

#if 0
    std::cout << sizeof(Date::day) << std::endl;
    std::cout << &today.day << std::endl;
#endif

    return 0;
}