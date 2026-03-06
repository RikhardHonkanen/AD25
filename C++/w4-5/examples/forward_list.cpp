#include <iostream>
#include <algorithm>
#include <forward_list>

int main(void)
{
    std::forward_list<int> list(5); // 5 i size

    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;
    list.assign({1, 8, 3, 3, 10});

    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::cout << list.front() << std::endl;
    list.pop_front();
    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    list.sort();
    list.unique();
    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    list.remove(8);
    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    list.push_front(1);
    list.push_front(100);
    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    list.remove_if([](const auto &elem)
                   { return (elem > 5); });
    std::for_each(list.begin(), list.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    return 0;
}
