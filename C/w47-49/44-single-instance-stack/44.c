/**
 * @file 44.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Implement a single instance stack module using a linked list to store data of type int.
 * The module shall be tested using assertion and provide the following functions:
 * A function to push a data element to the stack
 * A function to pop a data element from the stack
 * A function to get number of data elements stored in the stack
 * A function to clear the stack
 * @version 0.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "stack.h"
#include <stdio.h>
#include <assert.h>

void test_stack_push()
{
    int temp[4] = {10, 4, 22, 38};

    for (size_t i = 0; i < sizeof(temp) / sizeof(temp[0]); i++)
    {
        assert(stack_push(temp[i]) == 1);
        assert(i + 1 == stack_available());
    }

    stack_destroy();
    assert(0 == stack_available());
}

void test_stack_pop()
{
    int popped_value;
    int temp[2] = {71, 99};

    for (size_t i = 0; i < sizeof(temp) / sizeof(temp[0]); i++)
    {
        assert(stack_push(temp[i]));
        assert(i + 1 == stack_available());
    }

    assert(stack_pop(&popped_value));
    assert(1 == stack_available());
    assert(99 == popped_value);

    assert(stack_pop(&popped_value));
    assert(0 == stack_available());
    assert(71 == popped_value);

    assert(!stack_pop); // Stack should be empty
}

int main(void)
{
    test_stack_push();
    (void)printf("Tested stack push succesfully.\n");

    test_stack_pop();
    (void)printf("Tested stack pop succesfully.\n");

    (void)printf("All tests passed!\n");

    return 0;
};