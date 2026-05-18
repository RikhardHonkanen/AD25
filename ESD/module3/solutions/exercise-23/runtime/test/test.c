#include "stack.h"
#include "stdlib.h"
#include <unity.h>

#define SIZE 10

static stack_t *stack; // A stack pointer used in the tests

void setUp(void)
{
    stdlib_init();
    stack = stack_create(SIZE);
    TEST_ASSERT_NOT_NULL(stack);
    TEST_ASSERT_EQUAL_INT(SIZE, stack_size(stack));
    TEST_ASSERT_EQUAL_INT(0, stack_available(stack));
}

void tearDown(void)
{
    stack_destroy(&stack);
    TEST_ASSERT_NULL(stack);
    TEST_ASSERT_TRUE(stdlib_all_blocks_released()); // Ensure that all the allocated memory blocks were freed.
}

void test_stack_invalid_arguments(void)
{
    int value = 0;

    TEST_ASSERT_NULL(stack_create(0));

    TEST_ASSERT_EQUAL_INT(0, stack_size(NULL));

    TEST_ASSERT_EQUAL_INT(-1, stack_available(NULL));

    TEST_ASSERT_FALSE(stack_clear(NULL));

    TEST_ASSERT_FALSE(stack_push(NULL, value));

    TEST_ASSERT_FALSE(stack_pop(NULL, &value));
    TEST_ASSERT_FALSE(stack_pop(stack, NULL));
    TEST_ASSERT_FALSE(stack_pop(NULL, NULL));

    stdlib_set_malloc_status(false, true); // To enforce the first call to malloc in stack_create fails
    TEST_ASSERT_NULL(stack_create(SIZE));

    stdlib_set_malloc_status(true, false); // To enforce the second call to malloc in stack_create fails
    TEST_ASSERT_NULL(stack_create(SIZE));

    stack_t *temp = NULL;
    stack_destroy(&temp);
    stack_destroy(NULL);
}

void test_stack_push_clear(void)
{
    TEST_ASSERT_TRUE(stack_push(stack, 20));
    TEST_ASSERT_EQUAL_INT(1, stack_available(stack));

    TEST_ASSERT_TRUE(stack_push(stack, 200));
    TEST_ASSERT_EQUAL_INT(2, stack_available(stack));

    TEST_ASSERT_TRUE(stack_clear(stack));
    TEST_ASSERT_EQUAL_INT(0, stack_available(stack));
}

void test_stack_push_full(void)
{
    for (int i = 1; i <= SIZE; i++)
    {
        TEST_ASSERT_TRUE(stack_push(stack, i));
        TEST_ASSERT_EQUAL_INT(i, stack_available(stack));
    }

    TEST_ASSERT_FALSE(stack_push(stack, 100));
}

void test_stack_push_pop(void)
{
    for (int i = 1; i <= SIZE; i++)
    {
        TEST_ASSERT_TRUE(stack_push(stack, i));
        TEST_ASSERT_EQUAL_INT(i, stack_available(stack));
    }

    int value;
    for (int i = 1; i <= SIZE; i++)
    {
        TEST_ASSERT_TRUE(stack_pop(stack, &value));
        TEST_ASSERT_EQUAL_INT(SIZE + 1 - i, value);
        TEST_ASSERT_EQUAL_INT(SIZE - i, stack_available(stack));
    }

    TEST_ASSERT_FALSE(stack_pop(stack, &value));
}

int main(void)
{
    stack_memory_t memory = {.allocate = malloc, .release = free};

    stack_init(&memory);

    UNITY_BEGIN();

    RUN_TEST(test_stack_invalid_arguments);
    RUN_TEST(test_stack_push_clear);
    RUN_TEST(test_stack_push_full);
    RUN_TEST(test_stack_push_pop);

    return UNITY_END();
}