#include <stack>
#include <queue>
#include <array>
#include <iostream>

int main(void)
{
    std::stack<int> stack;

    stack.push(10);
    stack.push(100);

    std::cout << "Stack Size: " << stack.size() << std::endl;
    std::cout << "Stack Top Element: " << stack.top() << std::endl;

    stack.pop();

    std::cout << "Stack Size: " << stack.size() << std::endl;

    if (!stack.empty())
    {
        std::cout << "Stack Top Element: " << stack.top() << std::endl;
        stack.pop();
    }
    std::cout << "Stack Size: " << stack.size() << std::endl;

    /////////////////// Queue //////////////////////////////
    std::queue<int> queue;
    queue.push(100);
    queue.push(50);
    queue.push(1000);
    queue.push(800);
    queue.push(300);

    while (!queue.empty())
    {
        std::cout << queue.front() << "\t";
        queue.pop();
    }
    std::cout << std::endl
              << std::endl;

    /////////////////// Priority Queue //////////////////////////////
    std::priority_queue<int> priority_queue;
    std::array<int, 5> array{100, 50, 1000, 800, 300};

    std::cout << "The order the values are pushed to the priority queue: ";
    for (const auto &elem : array)
    {
        priority_queue.push(elem);
        std::cout << elem << "\t";
    }
    std::cout << std::endl;

    std::cout << "The order the values are stored in the priority queue: ";
    while (!priority_queue.empty())
    {
        std::cout << priority_queue.top() << "\t";
        priority_queue.pop();
    }
    std::cout << std::endl;

    return 0;
}
