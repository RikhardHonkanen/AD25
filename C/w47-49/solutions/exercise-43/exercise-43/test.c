#include <stdio.h>
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

int main(void)
{
    assert(queue_write(10));
    assert(1 == queue_available());
    assert(queue_write(20));
    assert(queue_write(30));
    assert(3 == queue_available());
    (void)printf("Testing enqueue... Passed!\n");

    int value;
    assert(queue_read(&value));
    assert(2 == queue_available());
    assert(10 == value);

    assert(queue_read(&value));
    assert(1 == queue_available());
    assert(20 == value);
    (void)printf("Testing dequeue... Passed!\n");

    queue_clear();
    assert(0 == queue_available());
    (void)printf("Testing clear... Passed!\n");

    (void)printf("----------------------------\n");
    (void)printf("All the tests passed!\n");

    return 0;
}