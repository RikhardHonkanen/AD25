#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Uncomment the following line to protect count
// #define RACE_FREE

static volatile int count = 0;
#ifdef RACE_FREE
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // The mutex to control access to the shared variable
#endif

void *increment(void *arg)
{
    for (int i = 0; i < 10000; i++)
    {
#ifdef RACE_FREE
        pthread_mutex_lock(&mutex);   // Lock the mutex
        count++;                      // There is no race. count is protected
        pthread_mutex_unlock(&mutex); // Unlock the mutex
#else
        count++; // There is a race condition on modifying count
#endif
    }
}

int main(void)
{
    pthread_t threads[5];

    for (int i = 0; i < sizeof(threads) / sizeof(pthread_t); i++)
    {
        if (0 != pthread_create(&threads[i], NULL, increment, NULL))
        {
            printf("Failed to create the thread #%d!\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < sizeof(threads) / sizeof(pthread_t); i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Count value: %d\n", count);

#ifdef RACE_FREE
    pthread_mutex_destroy(&mutex);
#endif

    return 0;
}