#include <vector>
#include <iostream>
#include <algorithm>

class Point
{
    int x, y;

public:
    Point(int _x = 0, int _y = 0) : x{_x}, y{_y} {}

    friend std::ostream &operator<<(std::ostream &out, const Point &point)
    {
        out << "(" << point.x << ", " << point.y << ")" << std::endl;
        return out;
    }
};

int main(void)
{
    std::vector<int> vector{2, 1, 5, 4, 3};
    std::cout << "Vector Size: " << vector.size() << std::endl;

    vector[0] = 8;
    std::for_each(vector.begin(), vector.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::sort(vector.begin(), vector.end(), [](auto a, auto b)
              { return (a > b); });

    std::for_each(vector.begin(), vector.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    vector.clear();
    std::cout << "Vector Size: " << vector.size() << std::endl;

    vector.resize(10);
    std::cout << "Vector Size: " << vector.size() << std::endl;
    std::for_each(vector.begin(), vector.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    vector.clear();
    std::cout << "Vector Size: " << vector.size() << std::endl;
    for (int i = 0; i < 10; i++)
    {
        vector.push_back(i);
    }
    std::for_each(vector.begin(), vector.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::vector<Point> points(5); // 5 is size
    std::cout << "Points Size: " << points.size() << std::endl;
    points[0] = Point{1, 2};
    std::cout << points[0];

    points.clear();
    std::cout << "Points Size: " << points.size() << std::endl;

    for (int i = 0; i < 5; i++)
    {
        points.emplace_back(i, i + 1);
    }

    std::for_each(points.begin(), points.end(), [](const auto &elem)
                  { std::cout << elem; });

    std::cout << "Points Size: " << points.size() << std::endl;
    points.pop_back();
    std::cout << "Points Size: " << points.size() << std::endl;

    return 0;
}
