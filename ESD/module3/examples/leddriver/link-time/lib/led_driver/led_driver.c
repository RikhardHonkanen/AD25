#include "led_driver.h"
#include "driver/gpio.h"

static gpio_num_t pin_num;
static bool initialized = false;

bool led_driver_init(int pin)
{
    initialized = false;

    if ((ESP_OK == gpio_reset_pin(pin)) &&
        (ESP_OK == gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT)) &&
        (ESP_OK == gpio_set_level(pin, 0)))
    {
        pin_num = pin;
        initialized = true;
    }

    return initialized;
}

bool led_driver_set_state(int state)
{
    bool status = false;

    if (initialized)
    {
        if ((state == 0) || (state == 1))
        {
            status = (ESP_OK == gpio_set_level(pin_num, state));
        }
    }

    return status;
}