#include "bsp.h"
#include "led_driver.h"

#define PIN_INVALID -1

static int pin_num = PIN_INVALID;

bool led_driver_init(int pin)
{
    pin_num = pin;

    return (bsp_pin_config(pin, BSP_PIN_INPUT_OUTPUT) && bsp_pin_write(pin, 0));
}

bool led_driver_set_state(int state)
{
    bool status = false;

    if (pin_num != PIN_INVALID)
    {
        if ((state == 0) || (state == 1))
        {
            if (bsp_pin_write(pin_num, state))
            {
                status = (state == bsp_pin_read(pin_num));
            }
        }
    }

    return status;
}