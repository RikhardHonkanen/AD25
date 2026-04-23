#include "led_driver.h"

#define PIN_INVALID -1

static led_driver_config_t config = {.pin.num = PIN_INVALID};

bool led_driver_init(const led_driver_config_t *ptr)
{
    config = *ptr;
    return (config.set_mode(config.pin.num, config.pin.mode) && config.set_state(config.pin.num, 0));
}

bool led_driver_set_state(int state)
{
    bool status = false;

    if (config.pin.num != PIN_INVALID)
    {
        if ((state == 0) || (state == 1))
        {
            if (config.set_state(config.pin.num, state))
            {
                status = (state == config.get_state(config.pin.num));
            }
        }
    }

    return status;
}