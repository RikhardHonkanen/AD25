/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of digital output configured using Config Tools.
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

#define ON 0
#define OFF 1

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

    uint8_t rgb = 0;

    while (1)
    {
        delay();

        switch (rgb)
        {
        case 0:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, ON);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, OFF);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, OFF);
            rgb = 1;
            break;
        case 1:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, OFF);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, ON);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, OFF);
            rgb = 2;
            break;
        case 2:
            GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, OFF);
            GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, OFF);
            GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, ON);
            rgb = 0;
            break;
        default:
            break;
        }
    }

    return 0;
}
