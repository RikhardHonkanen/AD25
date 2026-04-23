/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of creating tasks.
 *        Look at: https://www.freertos.org/Documentation/00-Overview
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

#define INTERVAL 500U

static void hello(void *param)
{
    (void)param;

    while (1)
    {
        printf("Hello world!\n");
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));
    }
}

// The task function to toggle the LED every 500ms.
// Instead of vTaskDelay, we have to use vTaskDelayUntil.
static void toggle(void *param)
{
    (void)param;

    printf("Initialize the %s task...\n", pcTaskGetName(NULL));

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT)); // Configure pin 4 as a digital input/output pin

    // Initialise xLastWakeTime with the current number of the system ticks.
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        // Toggle the LED connected to pin 4.
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4)));

        // Make a delay to simulate a time consuming task
        for (volatile int i = 0; i < 0xFFFFF; i++)
        {
            ;
        }

        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(INTERVAL));
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
    // Create the hello task at priority one
    if (pdPASS != xTaskCreate(hello, "hello", 2 * configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        print_error("Failed to create the ping task!");
    }

    // Create the toggle task at priority one
    if (pdPASS != xTaskCreate(toggle, "toggle", 2 * configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        print_error("Failed to create the pong task!");
    }
}
