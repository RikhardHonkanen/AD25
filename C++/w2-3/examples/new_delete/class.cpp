#include <new>
#include <iostream>

class Point
{
    int x, y;

public:
    Point(int _x = 0, int _y = 0) : x{_x}, y{_y} {}
    void *operator new(size_t size, const std::nothrow_t &tag)
    {
        std::cout << "The overloaded new" << std::endl;
        void *ptr = ::operator new(size, tag);
        return ptr;
    }

    void operator delete(void *ptr)
    {
        std::cout << "The overloaded delete" << std::endl;
        ::operator delete(ptr);
    }

    friend std::ostream &operator<<(std::ostream &out, const Point &point)
    {
        out << "(" << point.x << ", " << point.y << ")" << std::endl;
        return out;
    }
};

int main(void)
{
    Point *ptr{new (std::nothrow) Point{1, 2}};

    std::cout << *ptr;

    delete ptr;

    return 0;
}