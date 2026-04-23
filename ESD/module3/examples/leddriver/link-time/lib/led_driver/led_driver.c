#include "led_driver.h"
#include "driver/gpio.h"

#define PIN_INVALID -1

static gpio_num_t pin_num = PIN_INVALID;

bool led_driver_init(int pin)
{
    pin_num = pin;
    return ((ESP_OK == gpio_reset_pin(pin_num)) && (ESP_OK == gpio_set_direction(pin_num, GPIO_MODE_INPUT_OUTPUT)) && (ESP_OK == gpio_set_level(pin_num, 0)));
}

bool led_driver_set_state(int state)
{
    bool status = false;

    if (pin_num != PIN_INVALID)
    {
        if ((state == 0) || (state == 1))
        {
            if (ESP_OK == gpio_set_level(pin_num, state))
            {
                status = (state == gpio_get_level(pin_num));
            }
        }
    }

    return status;
}