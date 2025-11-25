/**
 * @file 43.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Implement a single instance queue module using a linked list to store data of type int.
 * The module shall be tested using assertion and provide the following functions.
 * A function to enqueue a value in the queue
 * A function to dequeue a value from the queue
 * A function to get number of the data elements stored in the queue
 * A function to delete all the data elements stored in the queue
 * @version 0.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "queue.h"
#include <stdio.h>
#include <assert.h>

void test_list_insert()
{
    int temp[4] = {10, 4, 22, 38};

    for (size_t i = 0; i < sizeof(temp) / sizeof(temp[0]); i++)
    {
        assert(enqueue_value(temp[i]) == 1);
        assert(i + 1 == queue_available());
    }

    queue_destroy();
    assert(0 == queue_available());
}

void test_list_remove()
{
    int temp[3] = {5, 71, 99};

    for (size_t i = 0; i < sizeof(temp) / sizeof(temp[0]); i++)
    {
        assert(enqueue_value(temp[i]));
        assert(i + 1 == queue_available());
    }

    assert(!dequeue_value(1000)); // Test non-existing
    assert(dequeue_value(71));
    assert(2 == queue_available());

    queue_destroy();
    assert(0 == queue_available());
}

int main(void)
{
    test_list_insert();
    (void)printf("Tested list insert successfully.\n");

    test_list_remove();
    (void)printf("Tested list remove successfully.\n");

    (void)printf("All tests passed!\n");
    return 0;
};