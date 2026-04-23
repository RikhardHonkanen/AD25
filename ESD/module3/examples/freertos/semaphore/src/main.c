/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using FreeRTOS semaphores to synchronize tasks.
 *        Look at: https://www.freertos.org/Documentation/00-Overview
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <freertos/FreeRTOS.h>

#define INTERVAL 500U

static SemaphoreHandle_t ping_sem, pong_sem;

// The task function to print Ping.
static void print_ping(void *param)
{
    (void)param;

    while (1)
    {
        // Sleep for 500 milliseconds.
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        assert(pdTRUE == xSemaphoreTake(ping_sem, portMAX_DELAY)); // Wait for signal from ping_sem.

        printf("Ping - ");

        assert(pdTRUE == xSemaphoreGive(pong_sem)); // Signal pong_sem to print Pong.
    }
}

// The task function to print Pong.
static void print_pong(void *param)
{
    (void)param;

    while (1)
    {
        assert(pdTRUE == xSemaphoreTake(pong_sem, portMAX_DELAY)); // Wait for signal from pong_sem.

        printf("Pong\n");

        assert(pdTRUE == xSemaphoreGive(ping_sem)); // Signal ping_sem to print Pong.
    }
}

static void print_error(const char *msg)
{
    while (1)
    {
        printf("%s\n", msg);
        vTaskDelay(pdMS_TO_TICKS(2 * INTERVAL));
    }
}

void app_main(void)
{
    // Initialize the binary semaphores
    ping_sem = xSemaphoreCreateCounting(1, 1); // xSemaphoreCreateMutex() or xSemaphoreCreateBinary()
    pong_sem = xSemaphoreCreateCounting(1, 0);

    if ((ping_sem == NULL) || (pong_sem == NULL))
    {
        print_error("Failed to create the semaphores!");
    }

    // Create the print_ping task at priority one
    if (pdPASS != xTaskCreate(print_ping, "ping", 2 * configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL))
    {
        print_error("Failed to create the ping task!");
    }

    // Create the print_pong task at priority one
    if (pdPASS != xTaskCreate(print_pong, "pong", 2 * configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL))
    {
        print_error("Failed to create the pong task!");
    }
}
