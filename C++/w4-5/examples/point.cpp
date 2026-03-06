#include <iostream>

#if 1
template <typename T>
class Point
{
    T x, y;

public:
    Point(T _x = 0, T _y = 0) : x{_x}, y{_y} {}
    template <typename U>
    friend std::ostream &operator<<(std::ostream &out, const Point<U> &point);
};

template <typename U>
std::ostream &operator<<(std::ostream &out, const Point<U> &point)
{
    out << "(" << point.x << ", " << point.y << ")" << std::endl;
    return out;
}
#else
template <typename T>
class Point
{
    T x, y;

public:
    Point(T _x = 0, T _y = 0) : x{_x}, y{_y} {}
    friend std::ostream &operator<<(std::ostream &out, const Point<T> &point)
    {
        out << "(" << point.x << ", " << point.y << ")" << std::endl;
        return out;
    }
};
#endif

int main(void)
{
    Point<int> p{1, 2};
    Point<double> q{1.2, 2.5};

    std::cout << p << q;

    return 0;
}
