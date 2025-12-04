#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void test_create_stacks(void)
{
    stack_t *stack = stack_create(10);
    assert(stack != NULL);

    assert(10 == stack_size(stack));
    assert(0 == stack_available(stack));

    assert(NULL == stack_create(0));
    assert(0 == stack_size(NULL));
    assert(-1 == stack_available(NULL));

    stack_destroy(&stack);
    assert(stack == NULL);

    stack_t *temp = stack_create(20);
    assert(temp != NULL);

    assert(20 == stack_size(temp));
    assert(0 == stack_available(temp));

    stack_destroy(&temp);
    assert(temp == NULL);
}

void test_stack_push_clear(void)
{
    stack_t *stack = stack_create(10);
    assert(stack != NULL);

    assert(10 == stack_size(stack));
    assert(0 == stack_available(stack));

    assert(stack_push(stack, 20));
    assert(1 == stack_available(stack));

    assert(stack_clear(stack));
    assert(0 == stack_available(stack));

    assert(!stack_clear(NULL));

    assert(!stack_push(NULL, 100));

    uint16_t size = stack_size(stack);
    for (int i = 1; i <= size; i++)
    {
        assert(stack_push(stack, i));
        assert(i == stack_available(stack));
    }
    assert(!stack_push(stack, 100));

    stack_destroy(&stack);
}

void test_stack_push_pop(void)
{
    stack_t *stack = stack_create(10);
    assert(stack != NULL);

    assert(10 == stack_size(stack));
    assert(0 == stack_available(stack));

    assert(stack_push(stack, 20));
    assert(1 == stack_available(stack));

    uint8_t data = 0;
    assert(stack_pop(stack, &data));
    assert(0 == stack_available(stack));
    assert(20 == data);

    assert(!stack_pop(stack, &data));

    assert(!stack_pop(NULL, &data));
    assert(!stack_pop(NULL, NULL));
    assert(stack_push(stack, 20));
    assert(1 == stack_available(stack));
    assert(!stack_pop(stack, NULL));

    stack_destroy(&stack);
}

int main(void)
{
    test_create_stacks();
    (void)printf("\nTesting create stacks... Passed\n");

    test_stack_push_clear();
    (void)printf("Testing push-clear... Passed\n");

    test_stack_push_pop();
    (void)printf("Testing push-pop... Passed\n");

    (void)printf("--------------------------------\n");
    (void)printf("All tests Passed!\n\n");

    return 0;
}
