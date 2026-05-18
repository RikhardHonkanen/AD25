#include "bsp.h"
#include <string.h>
#include <stddef.h>
#ifndef DEVENV
#include <driver/gpio.h>
#endif

typedef struct
{
    size_t pos;
    char buffer[BSP_CONSOLE_BUFSIZE];
} buffer_t;

static char input = 0;
static buffer_t output;
static bool console_status = true;

static int pin_num = -1;
static int pin_mode = 0;
static int pin_state = -1;
static bool pin_mode_status = false;
static bool pin_write_status = false;

int bsp_console_getchar(void)
{
    int chr = BSP_CONSOLE_EOF;

    if (console_status)
    {
        chr = input;
        input = BSP_CONSOLE_EOF;
    }

    return chr;
}

bool bsp_console_print(const char *str)
{
    if (console_status)
    {
        size_t length = strlen(str);
        memcpy(output.buffer + output.pos, str, length);
        output.pos = (output.pos + length) % BSP_CONSOLE_BUFSIZE;
    }

    return console_status;
}

const char *bsp_console_get_output(void)
{
    static char buffer[BSP_CONSOLE_BUFSIZE];

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, output.buffer);
    memset(&output, 0, sizeof(output));

    return buffer;
}

void bsp_console_set_input(char chr)
{
    input = chr;
}

void bsp_console_set_status(bool status)
{
    console_status = status;
}

bool bsp_pin_mode(int pin, int mode)
{
#ifndef DEVENV
    (void)gpio_reset_pin(pin);
    (void)gpio_set_direction(pin, mode);
#endif
    pin_num = pin;
    pin_mode = mode;
    return pin_mode_status;
}

bool bsp_pin_write(int pin, int state)
{
    pin_num = pin;
    if (pin_write_status)
    {
#ifndef DEVENV
        (void)gpio_set_level(pin, state);
#endif
        pin_state = state;
    }
    return pin_write_status;
}

int bsp_get_pin_state(void)
{
    return pin_state;
}

int bsp_get_pin_num(void)
{
    return pin_num;
}

int bsp_get_pin_mode(void)
{
    return pin_mode;
}

void bsp_set_mode_status(bool status)
{
    pin_mode_status = status;
}

void bsp_set_write_status(bool status)
{
    pin_write_status = status;
}