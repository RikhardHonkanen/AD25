#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <threads.h>

// To compile the code run: gcc -O1 volatile.c -o main && ./main

#if 1 // Change 1 to 0 and look at the ouput.
static bool status = false;
#else
volatile static bool status = false;
#endif

static int func(void *arg)
{
    (void)arg;

    while (!status)
    {
        ;
    }

    printf("Out of the loop\n");

    return 0;
}

int main(void)
{
    thrd_t thr;

    (void)thrd_create(&thr, func, NULL);

    sleep(2);
    status = true;

    (void)thrd_join(thr, NULL);

    return 0;
}