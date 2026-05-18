#include "cbuffer.h"
#include <zephyr/kernel.h>

K_SEM_DEFINE(mutex, 1, 1);                             // Binary semaphore acts like a mutex
K_SEM_DEFINE(filled_places, 0, CBUFFER_SIZE);          // A semaphore for the number of stored products
K_SEM_DEFINE(free_places, CBUFFER_SIZE, CBUFFER_SIZE); // A semaphore for the number of free places

static int head, tail;
static int buffer[CBUFFER_SIZE];

void cbuffer_init(void)
{
    head = tail = 0;
}

void cbuffer_store(int value)
{
    k_sem_take(&free_places, K_FOREVER);

    k_sem_take(&mutex, K_FOREVER);
    buffer[tail] = value;
    tail = (tail + 1) % CBUFFER_SIZE;
    k_sem_give(&mutex);

    k_sem_give(&filled_places);
}

int cbuffer_restore(void)
{
    k_sem_take(&filled_places, K_FOREVER);

    k_sem_take(&mutex, K_FOREVER);
    int value = buffer[head];
    head = (head + 1) % CBUFFER_SIZE;
    k_sem_give(&mutex);

    k_sem_give(&free_places);

    return value;
}

void cbuffer_deinit(void)
{
    head = tail = 0;
    k_sem_reset(&mutex);
    k_sem_reset(&free_places);
    k_sem_reset(&filled_places);
}