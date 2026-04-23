/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of making a LED connected to pin 4 (GPIO4) blinking every 0.5s using measuring time.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/gpio.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>

#define INTERVAL 500000 // 0.5s interval

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    //  You can also do it by running: pio run -t menuconfig
    //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));

    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT)); // Configure pin 4 as a digital IO

    int64_t current, prevoius = 0;

    while (1)
    {
        current = esp_timer_get_time();

        if (current - prevoius >= INTERVAL) // Timing is not guaranteed
        {
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4)));

            printf("LED state at %lld μs = %d\n", current, gpio_get_level(GPIO_NUM_4));

            prevoius = current;
        }

        // Simulate and run another task
        for (volatile int i = 0; i < 0xFFFF; i++)
        {
            ;
        }
    }
}
