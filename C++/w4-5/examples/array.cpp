#include <array>
#include <iostream>
#include <algorithm>

int main(void)
{
    std::array<int, 5> array{2, 1, 5, 4, 3};
    std::cout << "Array Size: " << array.size() << std::endl;

    std::for_each(array.begin(), array.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::sort(array.begin(), array.end());

    std::for_each(array.begin(), array.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::for_each(array.begin(), array.end(), [](auto &elem)
                  { elem *= 5; });

    std::for_each(array.begin(), array.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::transform(array.begin(), array.end(), array.begin(), [](const auto &elem)
                   { return (elem / 5); });
    std::for_each(array.begin(), array.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    // std::cout << array.at(100); /* out of range. An exception is thrown */

    std::string string{"abcde"};
    std::transform(string.begin(), string.end(), string.begin(), [](const auto &elem)
                   { return std::toupper(elem); });

    std::cout << string << std::endl;

    return 0;
}
