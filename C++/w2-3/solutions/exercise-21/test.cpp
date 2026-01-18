#include "stack.h"
#include <cassert>
#include <iostream>

int main(void)
{
    int value;
    Stack stck;

    assert(stck.push(10));
    assert(1 == stck.available());

    assert(stck.push(20));
    assert(2 == stck.available());

    assert(stck.pop(value));
    assert(1 == stck.available());
    assert(20 == value);

    assert(stck.pop(value));
    assert(0 == stck.available());
    assert(10 == value);

    stck.clear();

    assert(!stck.pop(value));
    assert(0 == stck.available());

    std::cout << "All tests passed!" << std::endl;

    return 0;
}