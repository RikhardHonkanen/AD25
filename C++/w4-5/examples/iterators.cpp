#include <deque>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <forward_list>

int main(void)
{
    int array[5]{10, 20, 30, 40, 50};
    int *ptr{array};

    while (ptr - array < 5)
    {
        std::cout << *ptr << "\t";
        ptr++;
    }
    std::cout << std::endl
              << std::endl;

    std::vector<int> vector{1, 2, 3, 4, 5};

    auto it{vector.begin()};
    while (it != vector.end())
    {
        std::cout << *it << "\t";
        it++;
    }
    std::cout << std::endl;

    auto rit{vector.rbegin()};
    while (rit != vector.rend())
    {
        std::cout << *rit << "\t";
        rit++;
    }
    std::cout << std::endl
              << std::endl;

    std::for_each(vector.begin(), vector.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::for_each(vector.rbegin(), vector.rend(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl
              << std::endl;

    ///////////////////// Deque ///////////////////
    std::deque<int> deque{10, 20, 30, 40, 50};
    std::cout << "Initial size of deque is " << deque.size() << std::endl;

    deque.push_back(60);
    deque.push_front(5);

    std::cout << std::endl
              << "Size of deque after push back and front is "
              << deque.size() << std::endl;

    std::copy(deque.begin(), deque.end(), std::ostream_iterator<int>{std::cout, "\t"});

    deque.clear();
    std::cout << std::endl
              << "Size of deque after clearing all values is "
              << deque.size() << std::endl;

    std::cout << std::endl
              << "Is the deque empty after clearing values? "
              << (deque.empty() ? "yes" : "no") << std::endl
              << std::endl;

    //////////////////////// Forward Lis ///////////////////
    std::forward_list<int> list{12, 45, 56, 23, 7};

    auto lit{list.begin()};
    *lit = 120;
    while (lit != list.end())
    {
        std::cout << *lit << "\t";
        lit++;
    }
    std::cout << std::endl;

    return 0;
}
