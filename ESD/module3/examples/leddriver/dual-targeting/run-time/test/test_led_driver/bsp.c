#include "bsp.h"
#ifndef DEVENV
#include <driver/gpio.h>
#endif

#define PIN_DISABLE 0
#define PIN_INVALID -1

static int pin_state = ~0;
static int pin_num = PIN_INVALID;
static int pin_mode = PIN_DISABLE;
static bool pin_config_status = false;
static bool pin_state_status = false;

bool bsp_pin_config(int pin, int mode)
{
#ifndef DEVENV
    (void)gpio_reset_pin(pin);
    (void)gpio_set_direction(pin, mode);
#endif
    pin_num = pin;
    pin_mode = mode;
    return pin_config_status;
}

bool bsp_pin_write(int pin, int state)
{
#ifndef DEVENV
    (void)gpio_set_level(pin, state);
#endif
    pin_num = pin;
    pin_state = state;
    return pin_state_status;
}

int bsp_pin_read(int pin)
{
    pin_num = pin;
    return (pin_state_status ? pin_state : !pin_state);
}

int bsp_get_pin_num(void)
{
    return pin_num;
}

int bsp_get_pin_mode(void)
{
    return pin_mode;
}

void bsp_set_config_status(bool status)
{
    pin_config_status = status;
}

void bsp_set_state_status(bool status)
{
    pin_state_status = status;
}