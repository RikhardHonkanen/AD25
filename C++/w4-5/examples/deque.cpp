#include <deque>
#include <iostream>
#include <algorithm>

int main(void)
{
    std::deque<int> dq(5);
    std::cout << "Dequq Size: " << dq.size() << std::endl;

    dq[0] = 100;
    std::for_each(dq.begin(), dq.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    dq.clear();
    std::cout << "Dequq Size: " << dq.size() << std::endl;

    for (int i = 0; i < 5; i++)
    {
        dq.push_back(i);
    }
    std::cout << "Dequq Size: " << dq.size() << std::endl;
    std::for_each(dq.begin(), dq.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl
              << std::endl;

    std::deque<int> deque{1, 2, 3, 4, 5};
    std::cout << "Dequq Size: " << deque.size() << std::endl;
    deque.push_back(100);
    deque.push_front(100);
    std::for_each(deque.begin(), deque.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    deque.pop_back();
    deque.pop_front();
    std::for_each(deque.begin(), deque.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    return 0;
}
