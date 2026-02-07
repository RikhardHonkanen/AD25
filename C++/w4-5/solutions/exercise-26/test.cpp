#include "queue.h"
#include <cassert>
#include <iostream>

int main(void)
{
    queue<int> q;

    assert(q.enqueue(10));
    assert(1 == q.available());

    int value;
    assert(q.dequeue(value));
    assert(0 == q.available());
    assert(10 == value);

    assert(q.enqueue(10));
    assert(1 == q.available());

    assert(q.enqueue(20));
    assert(2 == q.available());

    assert(q.dequeue(value));
    assert(1 == q.available());
    assert(10 == value);

    q.clear();
    assert(0 == q.available());
    assert(!q.dequeue(value));

    std::cout << "All tests passed!" << std::endl;

    return 0;
}