/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using FreeRTOS to exchange messages using a Queue.
 *        Look at: https://www.freertos.org/Documentation/00-Overview
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <esp_random.h>
#include <bootloader_random.h>
#include <freertos/FreeRTOS.h>

constexpr int BASE_TIME{500};
constexpr int STORE_CAPACITY{8};
constexpr int TASK_STACK_SIZE{2048};

struct Product
{
    uint32_t price;
    const char *name;
};

static QueueHandle_t store{nullptr};

static void producer(void *param)
{
    (void)param;

    printf("%s started ...\n", pcTaskGetName(nullptr));

    while (1)
    {
        Product product{14000U, "iPhone 14 Max Pro"};

        if (pdPASS != xQueueSend(store, &product, portMAX_DELAY))
        {
            printf("Failed to produce a product\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(BASE_TIME + (rand() % BASE_TIME)));
    }
}

static void consumer(void *param)
{
    (void)param;

    printf("%s started ...\n", pcTaskGetName(nullptr));

    int counter{0};
    Product product;

    while (1)
    {
        if (pdPASS == xQueueReceive(store, &product, portMAX_DELAY))
        {
            printf("%d) Product: {Price: %lu SEK, Name: %s}\r\n", ++counter, product.price, product.name);
        }

        vTaskDelay(pdMS_TO_TICKS(BASE_TIME + (rand() % BASE_TIME)));
    }
}

static void print_error(const char *msg)
{
    while (1)
    {
        printf("%s\n", msg);
        vTaskDelay(pdMS_TO_TICKS(2 * BASE_TIME));
    }
}

extern "C" void app_main(void)
{
    bootloader_random_enable();
    srand(esp_random());
    bootloader_random_disable();

    // Create the queue
    store = xQueueCreate(STORE_CAPACITY, sizeof(Product));
    if (store == nullptr)
    {
        print_error("Failed to create the store");
    }

    // Create the producer task
    if (pdPASS != xTaskCreate(producer, "Producer", TASK_STACK_SIZE, nullptr, 1, nullptr))
    {
        print_error("Failed to create the producer task");
    }

    // Create the consumer task
    if (pdPASS != xTaskCreate(consumer, "Consumer", TASK_STACK_SIZE, nullptr, 1, nullptr))
    {
        print_error("Failed to create the consumer task");
    }
}
