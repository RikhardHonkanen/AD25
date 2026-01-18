#include <cmath>
#include <cassert>
#include <iostream>

union Data // Members by default are public
{
    uint32_t dword{0};
    uint16_t word[2];
    uint8_t byte[4];

    uint32_t getDWord(void) { return dword; }

    uint16_t getWord(uint8_t index)
    {
        assert(index < 2);
        return word[index];
    }

    uint8_t getByte(uint8_t index)
    {
        assert(index < 4);
        return byte[index];
    }
};

struct Point // Members by default are public
{
    double x{0}, y{0};

    double distanceTo(const Point &p)
    {
        return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
    }
};

class Rectangle // Members by default are private
{
public:
    double width, height;
    double getArea(void) { return width * height; }
};

int main(void)
{
    Data data{0x12345678};
    std::cout << std::hex << "0x" << data.getDWord() << std::endl;
    std::cout << std::hex << "0x" << data.getWord(0) << std::endl;
    std::cout << std::hex << "0x" << static_cast<int>(data.getByte(0)) << std::endl;

    Point point{3, 4};
    std::cout << "Distance to (0, 0): " << point.distanceTo(Point{0, 0}) << std::endl;

    Rectangle rect{2, 5};
    std::cout << "Rect area: " << rect.getArea() << std::endl;

    return 0;
}