#include "led_driver.h"

#define INVALID -1

static int pin_state = 0;
static int pin_num = INVALID;
static int pin_status = false;

bool led_driver_init(int pin)
{
    if (pin_status)
    {
        pin_num = pin;
        pin_state = 0;
    }

    return pin_status;
}

bool led_driver_set_state(int state)
{
    pin_state = state;
    return pin_status;
}

// Helper functions
int led_driver_get_pin(void) { return pin_num; }
int led_driver_get_state(void) { return (pin_status ? pin_state : !pin_state); }
void led_driver_set_status(bool status) { pin_status = status; }
bool led_driver_is_init(void) { return (pin_num != INVALID); }