/**
 * @file test.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief To run the test: clear && g++ stack.cpp test.cpp -o test && ./test && rm test
 * @version 0.1
 * @date 2023-01-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stack.h"
#include <cassert>
#include <iostream>

static stack stck;

void test_stack_clear_push(void)
{
    for (int i = 1; i <= stack::SIZE; i++)
    {
        assert(stck.push(i));
        assert(i == stck.available());
    }

    stck.clear();
    assert(0 == stck.available());
}

void test_stack_push_pop(void)
{
    assert(stck.push(10));
    assert(1 == stck.available());

    int value = 0;
    assert(stck.pop(value));
    assert(0 == stck.available());
    assert(10 == value);
}

void test_stack_empty_full(void)
{
    int value = 0;
    assert(!stck.pop(value));
    assert(0 == value);

    for (int i = 1; i <= stack::SIZE; i++)
    {
        assert(stck.push(i));
        assert(i == stck.available());
    }

    assert(stck.isfull());
    assert(!stck.push(100));

    for (int i = stack::SIZE; i > 0; i--)
    {
        assert(stck.pop(value));
        assert(i == value);
        assert((i - 1) == stck.available());
    }

    assert(stck.isempty());
    assert(!stck.pop(value));
}

int main(void)
{
    test_stack_clear_push();
    test_stack_push_pop();
    test_stack_empty_full();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}