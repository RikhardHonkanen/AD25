#include "button.h"
#include "bsp.h"

typedef struct
{
    uint8_t debounced_state;
    uint8_t stable_state;
    uint8_t previous_raw_state;
    uint8_t pin_number;
} button_t;

static button_t button = {
    .debounced_state = BUTTON_UNINITITIALIZED,
    .stable_state = BUTTON_UNINITITIALIZED,
    .previous_raw_state = BUTTON_UNINITITIALIZED,
    .pin_number = -1,
};

static uint8_t consecutive = 1;

bool button_init(int pin)
{
    bool status = bsp_pin_mode(pin, 1, 0);

    if (status)
    {
        button.debounced_state = BUTTON_RELEASED;
        button.stable_state = BUTTON_RELEASED;
        button.previous_raw_state = BUTTON_RELEASED;
        button.pin_number = pin;
    }

    return status;
}

void button_update_state(void)
{
    uint8_t raw_state = bsp_pin_read(button.pin_number);

    if (raw_state == button.previous_raw_state)
    {
        consecutive++;
    }
    else
    {
        consecutive = 1;
    }

    button.previous_raw_state = raw_state;

    if (consecutive >= BUTTON_SAMPLES)
    {
        if (raw_state != button.stable_state)
        {
            button.stable_state = raw_state;

            if (raw_state == BUTTON_PRESSED)
            {
                button.debounced_state = BUTTON_FALLING_EDGE;
            }
            else
            {
                button.debounced_state = BUTTON_RISING_EDGE;
            }
        }
    }
}

int button_get_state(void)
{
    uint8_t state = button.debounced_state;
    if (state == BUTTON_FALLING_EDGE)
    {
        button.debounced_state = BUTTON_PRESSED;
    }
    else if (state == BUTTON_RISING_EDGE)
    {
        button.debounced_state = BUTTON_RELEASED;
    }

    return state;
}
