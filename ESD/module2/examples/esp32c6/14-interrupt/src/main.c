/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using an interrupt attached to pin 4 (GPIO0) connected to a push button.
 *        Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/gpio.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>

static volatile uint32_t count = 0; // count shall be volatile as it is being accessed in the interrupt handler

// Define the GPIO interrupt handler
static void gpio_isr_handler(void *arg)
{
    (void)arg; // To avoid the warning of unused variable

    ESP_ERROR_CHECK(gpio_intr_disable(GPIO_NUM_4)); // Disable the GPIO interrupt

    count++; // Increment the shared variable

    ESP_ERROR_CHECK(gpio_intr_enable(GPIO_NUM_4)); // Enable the GPIO interrupt
}

void app_main(void)
{
    // Exclude the Idle Task from the Task WDT
    //  You can also do it by running: pio run -t menuconfig
    //  Component Config -> ESP System Settings -> Watch CPU0 Idle Task
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandle()));

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_NEGEDGE));

    ESP_ERROR_CHECK(gpio_install_isr_service(0)); // Install the GPIO ISR handler service

    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_NUM_4, gpio_isr_handler, NULL)); // Add the ISR handler

    uint32_t prev_count = count;

    while (1)
    {
        if (prev_count != count)
        {
            ESP_ERROR_CHECK(gpio_intr_disable(GPIO_NUM_4)); // Disable the GPIO interrupt
            count %= 100;                                   // Change the shared variable
            ESP_ERROR_CHECK(gpio_intr_enable(GPIO_NUM_4));  // Enable the GPIO interrupt

            printf("Count = %ld\n", count);
            prev_count = count;
        }
    }

    ESP_ERROR_CHECK(gpio_isr_handler_remove(GPIO_NUM_4));
    gpio_uninstall_isr_service();
}
