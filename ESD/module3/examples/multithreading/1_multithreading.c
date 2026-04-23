#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS_NUM 5
#define RANGE_STEP 1000

typedef struct
{
    int min;
    int max;
} range_t;

void *accumulate(void *arg)
{
    const range_t range = *((range_t *)arg);
    int *sumptr = (int *)malloc(sizeof(int));
    if (sumptr != NULL)
    {
        *sumptr = 0;
        for (int i = range.min; i < range.max; i++)
        {
            *sumptr += i;
        }
    }

    pthread_exit(sumptr);
}

int main(void)
{
    range_t range[THREADS_NUM];
    pthread_t threads[THREADS_NUM];
    int *iptr[THREADS_NUM] = {NULL};

    for (int i = 0; i < THREADS_NUM; i++)
    {
        range[i].min = RANGE_STEP * i + 1;
        range[i].max = range[i].min + RANGE_STEP;
        if (0 != pthread_create(&threads[i], NULL, accumulate, (void *)&range))
        {
            printf("Failed to create the consumer thread %d!\n", i);
            exit(EXIT_FAILURE);
        }
    }

    int sum = 0;
    for (int i = 0; i < THREADS_NUM; i++)
    {
        pthread_join(threads[i], (void **)&iptr[i]);

        if (iptr[i] != NULL)
        {
            sum += *iptr[i];
            free(iptr[i]);
        }
    }

    printf("Sum: %d\n", sum);

    return 0;
}