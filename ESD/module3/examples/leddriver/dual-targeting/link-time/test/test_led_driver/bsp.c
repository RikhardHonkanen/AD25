#include "bsp.h"
#ifndef DEVENV
#include <driver/gpio.h>
#endif

static int pin_num = -1;
static int pin_mode = 0;
static int pin_state = -1;
static bool pin_mode_status = false;
static bool pin_write_status = false;

bool bsp_pin_mode(int pin, int mode)
{
#ifndef DEVENV
    (void)gpio_reset_pin(pin);
    (void)gpio_set_direction(pin, mode);
#endif
    pin_num = pin;
    pin_mode = mode;
    return pin_mode_status;
}

bool bsp_pin_write(int pin, int state)
{
    pin_num = pin;
    if (pin_write_status)
    {
#ifndef DEVENV
        (void)gpio_set_level(pin, state);
#endif
        pin_state = state;
    }
    return pin_write_status;
}

int bsp_get_pin_state(void)
{
    return pin_state;
}

int bsp_get_pin_num(void)
{
    return pin_num;
}

int bsp_get_pin_mode(void)
{
    return pin_mode;
}

void bsp_set_mode_status(bool status)
{
    pin_mode_status = status;
}

void bsp_set_write_status(bool status)
{
    pin_write_status = status;
}