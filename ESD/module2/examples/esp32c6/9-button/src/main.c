/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a bouncing button connected to pin 4 (GPIO4) and enabling the internal pull up resistor of the pin.
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>

#define LOW 0
#define HIGH 1

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    //  You can also do it by running: pio run -t menuconfig
    //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));

    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT)); // Configure pin 4 as a digital output pin

    ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY));

    int current_state;
    uint32_t count = 0;
    int previous_state = HIGH;
    uint32_t previous_count = 0;

    while (1)
    {
        current_state = gpio_get_level(GPIO_NUM_4);

        if (current_state != previous_state)
        {
            if (current_state == LOW)
            {
                count++;
            }
            previous_state = current_state;
        }
        else if (count != previous_count)
        {
            printf("count: %lu\n", count);
            previous_count = count;
        }
        else
        {
            ;
        }
    }
}
