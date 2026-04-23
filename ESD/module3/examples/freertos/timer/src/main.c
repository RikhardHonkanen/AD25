/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using FreeRTOS vTaskDelayUntil and Timer to run time-critical tasks.
 *        Connect a LED to pin 4 (GPIO0)
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
#include <freertos/timers.h>

#define INTERVAL 500U
#define STACK_SIZE 2048U

static TimerHandle_t xAutoReloadTimer, xOneShotTimer;

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

// The timers callback function
static void timer_callback(TimerHandle_t xTimer)
{
    static int count = 0;
    const int timer_id = (const int)pvTimerGetTimerID(xTimer);

    if (timer_id == 0)
    {
        printf("-- OneShotTimer was expired! --\n");
    }
    else
    {
        count++;
        printf("%d) AutoReloadTimer was expired!\n", count);
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
    // Create the toggle task at priority configMAX_PRIORITIES - 1
    if (pdPASS != xTaskCreate(toggle, "toggle", STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL))
    {
        print_error("Failed to create the toggle task!");
    }

    xOneShotTimer = xTimerCreate("OneShot", pdMS_TO_TICKS(10 * INTERVAL), pdFALSE, (void *)0, timer_callback);
    if (xOneShotTimer != NULL)
    {
        xTimerStart(xOneShotTimer, 0);
    }
    else
    {
        print_error("Failed to create the OneShot timer!");
    }

    xAutoReloadTimer = xTimerCreate("Reload", pdMS_TO_TICKS(2 * INTERVAL), pdTRUE, (void *)1, timer_callback);
    if (xAutoReloadTimer != NULL)
    {
        xTimerStart(xAutoReloadTimer, 0);
    }
    else
    {
        print_error("Failed to create the AutoReload timer!");
    }
}
