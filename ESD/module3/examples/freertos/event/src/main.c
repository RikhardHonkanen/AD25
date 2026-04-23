/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of event group, task creation and synchronization.
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

#define PING BIT0
#define PONG BIT1

static EventGroupHandle_t event_group;

// The task function to print Ping.
static void print_ping(void *param)
{
    (void)param;

    while (1)
    {
        // Sleep for 500 milliseconds.
        vTaskDelay(INTERVAL / portTICK_PERIOD_MS);

        (void)xEventGroupWaitBits(event_group, PING, pdTRUE, pdFALSE, portMAX_DELAY);

        printf("Ping - ");

        (void)xEventGroupSetBits(event_group, PONG);
    }
}

// The task function to print Pong.
static void print_pong(void *param)
{
    (void)param;

    while (1)
    {
        (void)xEventGroupWaitBits(event_group, PONG, pdTRUE, pdFALSE, portMAX_DELAY);

        printf("Pong\n");

        (void)xEventGroupSetBits(event_group, PING);
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
    // Create the event group
    event_group = xEventGroupCreate();
    (void)xEventGroupSetBits(event_group, PING);

    // Create the print_ping task at priority one
    if (pdPASS != xTaskCreate(print_ping, "ping", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        print_error("Failed to create the ping task!");
    }

    // Create the print_pong task at priority one
    if (pdPASS != xTaskCreate(print_pong, "pong", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        print_error("Failed to create the pong task!");
    }
}
