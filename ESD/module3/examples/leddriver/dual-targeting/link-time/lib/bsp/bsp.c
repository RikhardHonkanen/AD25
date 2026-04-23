#include "bsp.h"
#include <driver/gpio.h>

bool bsp_pin_config(int pin, int mode)
{
    return (ESP_OK == gpio_reset_pin(pin)) && (ESP_OK == gpio_set_direction(pin, mode));
}

bool bsp_pin_write(int pin, int state)
{
    return (ESP_OK == gpio_set_level(pin, state));
}

int bsp_pin_read(int pin)
{
    return gpio_get_level(pin);
}