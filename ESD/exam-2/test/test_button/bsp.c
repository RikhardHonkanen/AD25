#include "bsp.h"

static int pin_num = -1;
static int pin_mode = -1;
static int pin_pull = -1;
static int pin_state = BUTTON_RELEASED;

static bool pin_mode_status = true;

bool bsp_pin_mode(int pin, int mode, int pull)
{
    pin_num = pin;
    pin_mode = mode;
    pin_pull = pull;

    return pin_mode_status;
}

int bsp_pin_read(int pin)
{
    pin_num = pin;

    return pin_state;
}

void bsp_set_pin_state(int state) { pin_state = state; }

void bsp_set_mode_status(bool status) { pin_mode_status = status; }

int bsp_get_pin_num(void) { return pin_num; }

int bsp_get_pin_mode(void) { return pin_mode; }

int bsp_get_pin_pull(void) { return pin_pull; }