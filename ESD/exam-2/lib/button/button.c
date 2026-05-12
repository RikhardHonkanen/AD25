#include "button.h"
#include "bsp.h"

typedef struct
{
    uint8_t debounced_state;
    uint8_t previous_raw_state;
    uint8_t pin_number;

    bool falling_edge;
    bool rising_edge;

} button_t;

static button_t button = {0};

static uint8_t consecutive = 1;

bool button_init(int pin)
{
    button.pin_number = pin;

    button.debounced_state = BUTTON_RELEASED;
    button.previous_raw_state = BUTTON_RELEASED;

    button.falling_edge = false;
    button.rising_edge = false;

    return bsp_pin_mode(button.pin_number, 1, 0);
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
        if (raw_state != button.debounced_state)
        {
            if (raw_state == BUTTON_PRESSED)
            {
                button.falling_edge = true;
            }

            button.debounced_state = raw_state;
        }
    }
}

int button_get_state(void) { return button.debounced_state; }

bool button_get_falling_edge(void)
{
    bool edge = button.falling_edge;
    button.falling_edge = false;

    return edge;
}