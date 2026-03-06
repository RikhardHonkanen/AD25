#include "queue.h"
#include <cassert>
#include <iostream>

int main(void)
{
    Queue<int> q1;
    assert(0 == q1.available());

    assert(q1.enqueue(10));
    assert(q1.enqueue(20));
    assert(q1.enqueue(30));
    assert(3 == q1.available());

    int value;
    assert(q1.dequeue(value));
    assert(value == 10);
    assert(2 == q1.available());

    Queue<int> q2{std::move(q1)};
    assert(0 == q1.available());
    assert(2 == q2.available());

    q1 = std::move(q2);
    assert(2 == q1.available());
    assert(0 == q2.available());

    q1 = std::move(q1);
    assert(2 == q1.available());

    q1.clear();
    assert(0 == q1.available());

    std::cout << "All tests passed!" << std::endl;

    return 0;
}