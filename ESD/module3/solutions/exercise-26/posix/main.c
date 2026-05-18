#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "cbuffer.h"

#define CONSUMERS 4

void *consumer(void *arg)
{
    (void)arg;

    while (1)
    {
        usleep(100000 * (1 + rand() % CONSUMERS));
        printf("Thread ID: %lu, Value: %d\n", pthread_self(), cbuffer_restore());
    }

    pthread_exit(NULL);
}

int main(void)
{
    int value = 1;
    pthread_t consumers[CONSUMERS];

    cbuffer_init();
    srand(time(NULL));

    for (int i = 0; i < CONSUMERS; i++)
    {
        if (0 != pthread_create(&consumers[i], NULL, consumer, NULL))
        {
            printf("Failed to create the consumer thread %d!\n", i);
            exit(EXIT_FAILURE);
        }

        pthread_detach(consumers[i]);
    }

    while (1)
    {
        usleep(100000 * (1 + rand() % CONSUMERS));
        cbuffer_store(value);
        value++;
    }

    cbuffer_deinit();

    return 0;
}
