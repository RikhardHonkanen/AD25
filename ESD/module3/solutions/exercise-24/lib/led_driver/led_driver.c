#include "bsp.h"
#include "led_driver.h"

static int pin_num;
static bool initialized = false;

bool led_driver_init(int pin)
{
    initialized = false;

    if (bsp_pin_mode(pin, BSP_GPIO_MODE_OUTPUT) && bsp_pin_write(pin, 0))
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
            status = (bsp_pin_write(pin_num, state));
        }
    }

    return status;
}