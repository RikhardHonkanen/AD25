/**
 * @file exercise-23.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A circle is a shape which has a center point and a radius.
 *        A square is a shape which has a length(four equal sides).
 *        Every shape has a name and an area. Every point has x and y on the 2d cartesian coordinate system.
 *
 *          1. Make a Shape class and a Point class.
 *          2. Make a Circle class based on Shape.
 *              a. The class shall have a function to get area of a circle.
 *              b. It shall be possible to check if two circles are equal or not by overloading the == operator.
 *          3. Make a Square class based on Shape. The class shall have a function to get area of a square.
 *          4. Make a function to print area of any shape.
 *
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <string>
#include <iostream>

class Shape
{
    std::string name;

public:
    Shape(std::string _name) : name{_name} {}
    virtual double getArea(void) const = 0;
    std::string getName(void) const
    {
        return name;
    }
};

class Point
{
    int x, y;

public:
    Point(int m, int n) : x{m}, y{n} {};
    bool operator==(const Point &point) { return (x == point.x && y == point.y); }
};

class Circle : public Shape
{
    Point center;
    double radius;
    static constexpr double PI{3.1415};

public:
    Circle(int x, int y, double r, std::string name = "") : Shape{"Circle " + name}, center{x, y}, radius{r} {};
    bool operator==(const Circle &circle) { return (center == circle.center && radius == circle.radius); }
    double getArea(void) const override
    {
        return (PI * radius * radius);
    }
};

class Square : public Shape
{
    double length;

public:
    Square(double len, std::string name = "") : Shape{"Square " + name}, length{len} {};

    double getArea(void) const override
    {
        return (length * length);
    }
};

void printArea(const Shape &shape)
{
    std::cout << shape.getName() << " Area: " << shape.getArea() << std::endl;
}

int main(void)
{
    Circle a{0, 0, 10.0, "A"};
    printArea(a);

    Square b{10.0, "B"};
    printArea(b);

    Circle c{0, 0, 10.0, "C"};
    printArea(c);

    std::cout << a.getName() << " is " << ((a == c) ? "" : "not ") << "equal to " << c.getName() << std::endl;

    return 0;
}
