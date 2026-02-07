#include "stack.h"
#include <cassert>
#include <iostream>

void test_stack_push(void)
{
    stack<int> stck;

    assert(stck.push(10));
    assert(1 == stck.available());

    assert(stck.push(20));
    assert(2 == stck.available());
}

void test_stack_pop(void)
{
    stack<int> stck;

    assert(stck.push(10));
    assert(1 == stck.available());

    assert(stck.push(20));
    assert(2 == stck.available());

    int value;

    assert(stck.pop(value));
    assert(1 == stck.available());
    assert(20 == value);

    assert(stck.pop(value));
    assert(0 == stck.available());
    assert(10 == value);
}

void test_stack_average(void)
{
    stack<int> stck;

    assert(stck.push(10));
    assert(1 == stck.available());

    assert(stck.push(20));
    assert(2 == stck.available());

    stck.clear();

    int value;
    assert(!stck.pop(value));
    assert(0 == stck.available());
}

void test_stack_clear(void)
{
    stack<int> stck;

    assert(stck.push(10));
    assert(1 == stck.available());

    assert(stck.push(20));
    assert(2 == stck.available());

    assert(15 == stck.average());

    stck.clear();
    assert(0 == stck.average());

    stack<const char *> str_stck;
    assert(str_stck.push("Hello"));
    assert(1 == str_stck.available());

    assert(str_stck.push("World"));
    assert(2 == str_stck.available());

#if 0
    str_stck.average(); // Error! The average function is not available for const char *
#endif
}

int main(void)
{
    test_stack_push();
    std::cout << "Testing push... Passed!" << std::endl;

    test_stack_pop();
    std::cout << "Testing pop... Passed!" << std::endl;

    test_stack_average();
    std::cout << "Testing average... Passed!" << std::endl;

    test_stack_clear();
    std::cout << "Testing clear... Passed!" << std::endl;

    std::cout << "-------------------------" << std::endl;
    std::cout << "All tests passed!" << std::endl;

    return 0;
}