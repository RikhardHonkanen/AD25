#include "ldr.h"
#include "config.h"

int ldr::get_resistance_min(void)
{
    return CONFIG_RESISTANCE_MIN;
}

int ldr::get_resistance_max(void)
{
    return CONFIG_RESISTANCE_MAX;
}