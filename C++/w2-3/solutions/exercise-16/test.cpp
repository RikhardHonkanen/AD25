#include "stack.h"
#include <cassert>
#include <iostream>

void test_stack_push(stack_t *stck)
{
    stack::clear(stck);

    assert(stack::push(stck, 10));
    assert(1 == stack::available(stck));

    assert(stack::push(stck, 20));
    assert(2 == stack::available(stck));
}

void test_stack_pop(stack_t *stck)
{
    stack::clear(stck);

    assert(stack::push(stck, 10));
    assert(1 == stack::available(stck));

    assert(stack::push(stck, 20));
    assert(2 == stack::available(stck));

    int value;

    assert(stack::pop(stck, value));
    assert(1 == stack::available(stck));
    assert(20 == value);

    assert(stack::pop(stck, value));
    assert(0 == stack::available(stck));
    assert(10 == value);
}

void test_stack_clear(stack_t *stck)
{
    stack::clear(stck);

    assert(stack::push(stck, 10));
    assert(1 == stack::available(stck));

    assert(stack::push(stck, 20));
    assert(2 == stack::available(stck));

    stack::clear(stck);

    int value;
    assert(!stack::pop(stck, value));
    assert(0 == stack::available(stck));
}

int main(void)
{
    stack_t *stck = stack::create();
    assert(stck != nullptr);

    test_stack_push(stck);
    std::cout << "Testing push... Passed!" << std::endl;

    test_stack_pop(stck);
    std::cout << "Testing pop... Passed!" << std::endl;

    test_stack_clear(stck);
    std::cout << "Testing clear... Passed!" << std::endl;

    std::cout << "-------------------------" << std::endl;
    std::cout << "All tests passed!" << std::endl;

    stack::destroy(stck);
    assert(stck == nullptr);

    return 0;
}