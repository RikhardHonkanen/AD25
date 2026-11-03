/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of digital input.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#gpio-driver
 *
 * @version 0.1
 * @date 2025-10-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    printf("\r\nPress SW3 on the board\r\n");

    uint32_t prev_state = 1, counter = 0;

    while (1)
    {
        uint32_t current_state = GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);

        if (prev_state != current_state)
        {
            prev_state = current_state;

            if (current_state == 0)
            {
                printf("%lu) Pressed\r\n", ++counter);
            }
        }
    }

    return 0;
}
