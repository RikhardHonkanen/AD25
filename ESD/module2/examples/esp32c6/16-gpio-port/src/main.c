/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of writing to multiple pins in a port simultaneously
 * @version 0.1
 * @date 2025-05-04
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <unistd.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "soc/gpio_struct.h"

void app_main(void)
{
    const int pins[] = {GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7};

    // Configure output GPIO pins
    gpio_config_t config = {
        .pin_bit_mask = 0,                    // Bitmask for output pins
        .mode = GPIO_MODE_OUTPUT,             // Set as output
        .intr_type = GPIO_INTR_DISABLE,       // Disable interrupt
        .pull_up_en = GPIO_PULLUP_DISABLE,    // Disable pull-up resistors
        .pull_down_en = GPIO_PULLDOWN_DISABLE // Disable pull-down resistors
    };

    for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++)
    {
        config.pin_bit_mask |= (1ULL << pins[i]);
    }

    gpio_config(&config); // Configure output pins

    while (1)
    {
        // Write HIGH (1) to all the pins simultaneously using the GPIO_OUT_REG
        GPIO.out_w1ts.val = config.pin_bit_mask;

        sleep(1); // Delay for 1 second

        // Write LOW (0) to all the pins simultaneously using the GPIO_OUT_REG
        GPIO.out_w1tc.val = config.pin_bit_mask;

        sleep(1); // Delay for 1 second
    }
}