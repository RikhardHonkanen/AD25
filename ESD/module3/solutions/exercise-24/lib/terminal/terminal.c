#include "bsp.h"
#include <string.h>
#include "terminal.h"
#include "led_driver.h"

#define BUFLEN (2 + ((sizeof(TERMINAL_DONE_MSG) > sizeof(TERMINAL_FAILED_MSG)) ? sizeof(TERMINAL_DONE_MSG) : sizeof(TERMINAL_FAILED_MSG)))

static bool status = false;
static bool show_menu = false;

bool terminal_init(void)
{
    show_menu = true;
    status = led_driver_init(TERMINAL_LED_PIN);
    return status;
}

bool terminal_run(void)
{
    if (status)
    {
        if (show_menu)
        {
            show_menu = false;
            status = bsp_console_print(TERMINAL_COMMAND_MSG);
        }

        if (status)
        {
            int chr = bsp_console_getchar();

            if ((chr == '\n') || (chr == TERMINAL_LED_ON) || (chr == TERMINAL_LED_OFF))
            {
                char buffer[BUFLEN] = {chr};

                if (chr != '\n')
                {
                    strcat(buffer, led_driver_set_state((chr == TERMINAL_LED_ON) ? 1 : 0) ? TERMINAL_DONE_MSG : TERMINAL_FAILED_MSG);
                }

                status = bsp_console_print(buffer);
                show_menu = true;
            }
        }
    }

    return status;
}