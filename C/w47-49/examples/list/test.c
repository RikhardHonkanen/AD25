#include "list.h"
#include <stdio.h>
#include <assert.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

void test_list_insert(void)
{
    int array[4] = {10, 2, 8, 20};

    for (size_t i = 0; i < ARRAY_LENGTH(array); i++)
    {
        assert(list_insert(array[i]));
        assert(i + 1 == list_available());
    }

    list_clear();
    assert(0 == list_available());
}

void test_list_search(void)
{
    assert(!list_search(40)); // Test for a non-existing value

    int array[3] = {30, 5, 10};

    for (size_t i = 0; i < ARRAY_LENGTH(array); i++)
    {
        assert(list_insert(array[i]));
        assert(i + 1 == list_available());
    }

    assert(list_search(5));
    assert(list_search(10));

    assert(!list_search(40)); // Test for a non-existing value

    list_clear();
    assert(0 == list_available());
}

void test_list_remove(void)
{
    int array[4] = {10, 2, -8, 20};

    for (size_t i = 0; i < ARRAY_LENGTH(array); i++)
    {
        assert(list_insert(array[i]));
        assert(i + 1 == list_available());
    }

    assert(!list_remove(33)); // Test for a non-existing value

    assert(list_remove(2));

    assert(3 == list_available());
    assert(!list_search(2));

    list_clear();
    assert(0 == list_available());
}

int main(void)
{
    test_list_insert();
    (void)printf("\nTesting insert... Passed\n");

    test_list_search();
    (void)printf("Testing find... Passed\n");

    test_list_remove();
    (void)printf("Testing remove... Passed\n");

    (void)printf("-----------------------------\n");
    (void)printf("All tests Passed!\n\n");

    return 0;
}
