#include "cbuffer.h"
#include <stdlib.h>
#include <freertos/FreeRTOS.h>

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t free_places;
static SemaphoreHandle_t filled_places;

static int head, tail;
static int buffer[CBUFFER_SIZE];

bool cbuffer_init(void)
{
    head = tail = 0;
    mutex = xSemaphoreCreateCounting(1, 1);                             // Binary semaphore acts like a mutex
    filled_places = xSemaphoreCreateCounting(CBUFFER_SIZE, 0);          // A semaphore for the number of stored products
    free_places = xSemaphoreCreateCounting(CBUFFER_SIZE, CBUFFER_SIZE); // A semaphore for the number of free places

    return ((mutex != NULL) && (filled_places != NULL) && (free_places != NULL));
}

void cbuffer_store(int value)
{
    xSemaphoreTake(free_places, portMAX_DELAY);

    xSemaphoreTake(mutex, portMAX_DELAY);
    buffer[tail] = value;
    tail = (tail + 1) % CBUFFER_SIZE;
    xSemaphoreGive(mutex);

    xSemaphoreGive(filled_places);
}

int cbuffer_restore(void)
{
    xSemaphoreTake(filled_places, portMAX_DELAY);

    xSemaphoreTake(mutex, portMAX_DELAY);
    int value = buffer[head];
    head = (head + 1) % CBUFFER_SIZE;
    xSemaphoreGive(mutex);

    xSemaphoreGive(free_places);

    return value;
}

void cbuffer_deinit(void)
{
    head = tail = 0;
    vSemaphoreDelete(mutex);
    vSemaphoreDelete(free_places);
    vSemaphoreDelete(filled_places);
}
