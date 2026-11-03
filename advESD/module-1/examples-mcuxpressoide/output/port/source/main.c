/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of gpio pins in a port.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#gpio-driver
 *
 * @version 0.1
 * @date 2025-10-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"

/* Make a delay */
static void delay(void)
{
    for (volatile int i = 0; i < 0x9FFFFF; i++)
    {
        __asm("NOP"); /* No Operation */
    }
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    // A mask for the led pins in the GPIO4.
    const uint32_t mask = (1 << BOARD_LED_RED_GPIO_PIN) |
        		          (1 << BOARD_LED_GREEN_GPIO_PIN) |
						  (1 << BOARD_LED_BLUE_GPIO_PIN);

    GPIO_PortSet(GPIO4, mask); // Turn the active low leds off

    while (1)
    {
        delay();

        // Toggle the led pins
        GPIO_PortToggle(GPIO4, mask);
    }

    return 0;
}
