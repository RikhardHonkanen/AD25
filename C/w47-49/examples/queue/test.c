#include <stdio.h>
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

int main(void)
{
    queue_t *q1 = queue_create();
    assert(q1 != NULL);
    assert(0 == queue_available(q1));

    queue_t *q2 = queue_create();
    assert(q2 != NULL);
    assert(0 == queue_available(q2));
    (void)printf("Testing create... Passed!\n");

    assert(queue_write(q1, 10));
    assert(1 == queue_available(q1));
    assert(queue_write(q1, 20));
    assert(queue_write(q1, 30));
    assert(3 == queue_available(q1));

    assert(queue_write(q2, 100));
    assert(1 == queue_available(q2));
    assert(queue_write(q2, 200));
    assert(queue_write(q2, 300));
    assert(3 == queue_available(q2));
    (void)printf("Testing enqueue... Passed!\n");

    int value;
    assert(queue_read(q1, &value));
    assert(2 == queue_available(q1));
    assert(10 == value);

    assert(queue_read(q1, &value));
    assert(1 == queue_available(q1));
    assert(20 == value);

    assert(queue_read(q2, &value));
    assert(2 == queue_available(q2));
    assert(100 == value);

    assert(queue_read(q2, &value));
    assert(1 == queue_available(q2));
    assert(200 == value);
    (void)printf("Testing dequeue... Passed!\n");

    queue_destroy(&q1);
    assert(q1 == NULL);

    queue_destroy(&q2);
    assert(q2 == NULL);

    (void)printf("Testing destroy... Passed!\n");

    (void)printf("----------------------------\n");
    (void)printf("All the tests passed!\n");

    return 0;
}