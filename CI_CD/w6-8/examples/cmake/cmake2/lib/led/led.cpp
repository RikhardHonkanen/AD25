#include "led.h"
#include "config.h"

int led::get_light_min(void)
{
    return CONFIG_LIGHT_MIN;
}

int led::get_light_max(void)
{
    return CONFIG_LIGHT_MAX;
}