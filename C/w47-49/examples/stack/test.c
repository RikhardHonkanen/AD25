#include "stack.h"
#include <stdio.h>
#include <assert.h>

void test_stack_clear_push(void)
{
    stack_clear();

    assert(0 == stack_available());

    for (int i = 0; i < STACK_SIZE; i++)
    {
        assert(stack_push(i));
        assert(i + 1 == stack_available());
    }

    stack_clear();
    assert(0 == stack_available());
}

void test_stack_push_pop(void)
{
    stack_clear();

    assert(stack_push(10));
    assert(1 == stack_available());

    int value = 0;
    assert(stack_pop(&value));
    assert(0 == stack_available());
    assert(10 == value);
}

void test_stack_empty_full(void)
{
    stack_clear();

    int value = 0;
    assert(!stack_pop(&value));
    assert(0 == value);

    for (int i = 1; i <= STACK_SIZE; i++)
    {
        assert(stack_push(i));
        assert(i == stack_available());
    }

    assert(!stack_push(100));

    for (int i = STACK_SIZE; i > 0; i--)
    {
        assert(stack_pop(&value));
        assert(i == value);
        assert(i - 1 == stack_available());
    }

    assert(!stack_pop(&value));
}

int main(void)
{
    test_stack_clear_push();
    (void)printf("\nTesting clear-push... Passed\n");

    test_stack_push_pop();
    (void)printf("Testing push-pop... Passed\n");

    test_stack_empty_full();
    (void)printf("Testing empty-full... Passed\n");

    (void)printf("-----------------------------\n");
    (void)printf("All tests Passed!\n\n");

    return 0;
}
