#include "led_driver.h"

static bool initialized = false;
static led_driver_config_t config;

bool led_driver_init(const led_driver_config_t *ptr)
{
    if (ptr->set_mode(ptr->pin.num, ptr->pin.mode) && ptr->set_state(ptr->pin.num, 0))
    {
        config = *ptr;
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
            status = config.set_state(config.pin.num, state);
        }
    }

    return status;
}