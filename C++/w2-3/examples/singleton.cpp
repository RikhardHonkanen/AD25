/**
 * @file singleton.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A singleton Point class
 * @version 0.1
 * @date 2023-01-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>

class Point
{
    int x, y;

    Point(const Point &) = delete;
    Point(int m, int n) : x{m}, y{n} {}

public:
    static Point *handle(int m = 0, int n = 0)
    {
        static Point point{m, n};
        return &point;
    }

    void print(void) const
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

void func(void)
{
    const Point *point = Point::handle();
    std::cout << point << std::endl;
    point->print();
}

int main(void)
{
    Point *point = Point::handle(2, 3);
    std::cout << point << std::endl;
    point->print();

    func();

    return 0;
}