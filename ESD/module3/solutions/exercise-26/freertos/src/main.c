#include <stdio.h>
#include "cbuffer.h"
#include <esp_random.h>
#include <bootloader_random.h>
#include <freertos/FreeRTOS.h>

#define DELAY 100
#define PRIORITY 1
#define CONSUMERS 4
#define STACK_SIZE 2048

static void consumer(void *param)
{
    const int ID = (int)param;
    const char *name = pcTaskGetName(NULL);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(DELAY * (1 + rand() % CONSUMERS)));
        printf("%s ID: %d, Value: %d\n", name, ID, cbuffer_restore());
    }
}

static void print_error(const char *msg)
{
    while (1)
    {
        printf("%s\n", msg);
        vTaskDelay(pdMS_TO_TICKS(10 * DELAY));
    }
}

void app_main(void)
{
    if (!cbuffer_init())
    {
        print_error("Failed to init cbuffer");
    }

    // Create the consumer tasks
    for (int i = 1; i <= CONSUMERS; i++)
    {
        if (pdPASS != xTaskCreate(consumer, "Consumer", STACK_SIZE, (void *)i, PRIORITY, NULL))
        {
            print_error("Failed to create the consumer task");
        }
    }

    bootloader_random_enable();
    srand(esp_random());
    bootloader_random_disable();

    int value = 0;

    while (1)
    {
        value++;
        cbuffer_store(value);
        vTaskDelay(pdMS_TO_TICKS(DELAY * (1 + rand() % CONSUMERS)));
    }
}