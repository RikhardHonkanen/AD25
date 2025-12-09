/**
 * @file test.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief
 * @version 0.1
 * @date 2025-12-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "util.h"
#include <stdio.h>
#include <assert.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static void insert_into_queue(queue_t *queue, int *array, size_t len)
{
    for (ssize_t i = 0; i < len; i++)
    {
        assert(queue_insert(queue, array[i]));
    }
}

void test_queue_create(void)
{
    queue_t *queue = queue_create(5);
    assert(queue != NULL);
    assert(5 == queue_size(queue));

    assert(-1 == queue_count(NULL));
    assert(0 == queue_count(queue));

    queue_t *temp = queue_create(3);
    assert(temp != NULL);
    assert(0 == queue_count(temp));

    queue_destroy(&temp);
    assert(temp == NULL);

    queue_destroy(&queue);
    assert(queue == NULL);
}

void test_queue_resize()
{
    queue_t *queue = queue_create(5);
    queue_resize(queue, 4);
    assert(4 == queue_size(queue));

    int data[] = {10, 2, 8, 20};
    insert_into_queue(queue, data, ARRAY_SIZE(data));
    queue_resize(queue, 3);
    assert(3 == queue_size(queue));

    queue_destroy(&queue);
    assert(queue == NULL);
}

void test_queue_insert(void)
{
    queue_t *queue = queue_create(5);
    assert(queue != NULL);
    assert(0 == queue_count(queue));

    int data[] = {10, 2, 8, 20};
    insert_into_queue(queue, data, ARRAY_SIZE(data));

    assert(!queue_insert(NULL, 30));

    queue_destroy(&queue);
    assert(queue == NULL);
}

void test_queue_empty()
{
    queue_t *queue = queue_create(3);

    int data[] = {10, 2, 8, 20};
    insert_into_queue(queue, data, ARRAY_SIZE(data));

    queue_empty(queue);
    assert(0 == queue_count(queue));

    queue_destroy(&queue);
    assert(queue == NULL);
}

void test_dequeue()
{
    queue_t *queue = queue_create(5);

    int data[] = {10, 2, 8, 20};
    insert_into_queue(queue, data, ARRAY_SIZE(data));

    int *value;
    dequeue(queue, value);
    assert(*value == 10);

    queue_destroy(&queue);
    assert(queue == NULL);
}

int main(void)
{
    test_queue_create();
    (void)printf("\nTesting create... Passed\n");

    test_queue_resize();
    (void)printf("\nTesting resize... Passed\n");

    test_queue_insert();
    (void)printf("\nTesting insert... Passed\n");

    test_queue_empty();
    (void)printf("\nTesting empty... Passed\n");

    test_dequeue();
    (void)printf("\nTesting dequeue... Passed\n");

    return 0;
};