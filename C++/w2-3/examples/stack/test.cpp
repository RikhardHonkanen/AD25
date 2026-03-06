#include "stack.h"
#include <cassert>
#include <iostream>

void test_stack_clear_push(void)
{
    stack::clear();

    assert(0 == stack::available());

    for (int i = 0; i < STACK_SIZE; i++)
    {
        assert(stack::push(i));
        assert(i + 1 == stack::available());
    }

    stack::clear();
    assert(0 == stack::available());
}

void test_stack_push_pop(void)
{
    stack::clear();

    assert(stack::push(10));
    assert(1 == stack::available());

    int value = 0;
    assert(stack::pop(value));
    assert(0 == stack::available());
    assert(10 == value);
}

void test_stack_empty_full(void)
{
    stack::clear();

    int value = 0;
    assert(!stack::pop(value));
    assert(0 == value);

    for (int i = 1; i <= STACK_SIZE; i++)
    {
        assert(stack::push(i));
        assert(i == stack::available());
    }

    assert(!stack::push(100));

    for (int i = STACK_SIZE; i > 0; i--)
    {
        assert(stack::pop(value));
        assert(i == value);
        assert(i - 1 == stack::available());
    }

    assert(!stack::pop(value));
}

int main(void)
{
    test_stack_clear_push();
    std::cout << "\nTesting clear-push... Passed\n";

    test_stack_push_pop();
    std::cout << "Testing push-pop... Passed\n";

    test_stack_empty_full();
    std::cout << "Testing empty-full... Passed\n";

    std::cout << "-----------------------------\n";
    std::cout << "All tests Passed!\n\n";

    return 0;
}
