#include <stdlib.h>
#include "cbuffer.h"
#include <semaphore.h>

static sem_t mutex;
static sem_t free_places;
static sem_t filled_places;

static int head, tail;
static int buffer[CBUFFER_SIZE];

void cbuffer_init(void)
{
    sem_init(&mutex, 0, 1);                  // Binary semaphore acts like a mutex
    sem_init(&filled_places, 0, 0);          // A semaphore for the number of stored products
    sem_init(&free_places, 0, CBUFFER_SIZE); // A semaphore for the number of free places
    head = tail = 0;
}

void cbuffer_store(int value)
{
    sem_wait(&free_places);

    sem_wait(&mutex);
    buffer[tail] = value;
    tail = (tail + 1) % CBUFFER_SIZE;
    sem_post(&mutex);

    sem_post(&filled_places);
}

int cbuffer_restore(void)
{
    sem_wait(&filled_places);

    sem_wait(&mutex);
    int value = buffer[head];
    head = (head + 1) % CBUFFER_SIZE;
    sem_post(&mutex);

    sem_post(&free_places);

    return value;
}

void cbuffer_deinit(void)
{
    sem_destroy(&mutex);
    sem_destroy(&filled_places);
    sem_destroy(&free_places);
    head = tail = 0;
}
