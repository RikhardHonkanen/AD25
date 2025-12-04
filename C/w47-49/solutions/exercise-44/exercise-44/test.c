#include "stack.h"
#include <stdio.h>
#include <assert.h>

void test_stack_push(void)
{
    assert(stack_push(10));
    assert(1 == stack_available());

    assert(stack_push(20));
    assert(2 == stack_available());

    stack_clear();
    assert(0 == stack_available());
}

void test_stack_pop(void)
{
    assert(stack_push(10));
    assert(1 == stack_available());

    assert(stack_push(20));
    assert(2 == stack_available());

    int value;
    assert(stack_pop(&value));
    assert(1 == stack_available());
    assert(value == 20);

    assert(stack_pop(&value));
    assert(0 == stack_available());
    assert(value == 10);

    assert(!stack_pop(NULL));

    stack_clear();
    assert(!stack_pop(&value));
    assert(0 == stack_available());
}

void test_stack_clear(void)
{
    assert(stack_push(10));
    assert(1 == stack_available());

    assert(stack_push(20));
    assert(2 == stack_available());

    stack_clear();
    assert(0 == stack_available());
}

int main(void)
{
    test_stack_push();
    (void)printf("Testing push... Passed!\n");

    test_stack_pop();
    (void)printf("Testing pop... Passed!\n");

    test_stack_clear();
    (void)printf("Testing clear... Passed!\n");

    (void)printf("---------------------------\n");
    (void)printf("All the tests passed!\n");

    return 0;
}
