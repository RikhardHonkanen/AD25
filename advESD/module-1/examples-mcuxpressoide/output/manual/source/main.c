/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of digital output.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#port-port-control-and-interrupts
 *                 https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#gpio-driver
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
#include "fsl_port.h"
#include "clock_config.h"

#define ON 0
#define OFF 1
#define LED_PORT PORT4
#define LED_GPIO GPIO4
#define LED_RED_PIN 18
#define LED_GREEN_PIN 19
#define LED_BLUE_PIN 17

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

    const port_pin_config_t port_pin_config = {
        .pullSelect = kPORT_PullDisable,                   /* Internal pull-up/down resistor is disabled */
        .pullValueSelect = kPORT_LowPullResistor,          /* Low internal pull resistor value is selected. */
        .slewRate = kPORT_FastSlewRate,                    /* Fast slew rate is configured */
        .passiveFilterEnable = kPORT_PassiveFilterDisable, /* Passive input filter is disabled */
        .openDrainEnable = kPORT_OpenDrainDisable,         /* Open drain output is disabled */
        .driveStrength = kPORT_LowDriveStrength,           /* Low drive strength is configured */
        .mux = kPORT_MuxAsGpio,                            /* Pin is configured as GPIO */
        .inputBuffer = kPORT_InputBufferEnable,            /* Digital input buffer enabled */
        .invertInput = kPORT_InputNormal,                  /* Digital input is not inverted */
        .lockRegister = kPORT_UnlockRegister,              /* Pin Control Register is not locked */
    };

    /* Configure the pins as GPIO pins */
    CLOCK_EnableClock(kCLOCK_Port4); // Enables clock for PORT4
    PORT_SetPinConfig(PORT4, LED_RED_PIN, &port_pin_config);
    PORT_SetPinConfig(PORT4, LED_GREEN_PIN, &port_pin_config);
    PORT_SetPinConfig(PORT4, LED_BLUE_PIN, &port_pin_config);

    const gpio_pin_config_t gpio_pin_config = {kGPIO_DigitalOutput, OFF};

    /* Config the pins as digital output pins */
    CLOCK_EnableClock(kCLOCK_Gpio4); // Enables clock for GPIO4
    GPIO_PinInit(LED_GPIO, LED_RED_PIN, &gpio_pin_config);
    GPIO_PinInit(LED_GPIO, LED_GREEN_PIN, &gpio_pin_config);
    GPIO_PinInit(LED_GPIO, LED_BLUE_PIN, &gpio_pin_config);

    uint8_t rgb = 0;

    while (1)
    {
        delay();

        switch (rgb)
        {
        case 0:
            GPIO_PinWrite(LED_GPIO, LED_RED_PIN, ON);
            GPIO_PinWrite(LED_GPIO, LED_GREEN_PIN, OFF);
            GPIO_PinWrite(LED_GPIO, LED_BLUE_PIN, OFF);
            rgb = 1;
            break;
        case 1:
            GPIO_PinWrite(LED_GPIO, LED_RED_PIN, OFF);
            GPIO_PinWrite(LED_GPIO, LED_GREEN_PIN, ON);
            GPIO_PinWrite(LED_GPIO, LED_BLUE_PIN, OFF);
            rgb = 2;
            break;
        case 2:
            GPIO_PinWrite(LED_GPIO, LED_RED_PIN, OFF);
            GPIO_PinWrite(LED_GPIO, LED_GREEN_PIN, OFF);
            GPIO_PinWrite(LED_GPIO, LED_BLUE_PIN, ON);
            rgb = 0;
            break;
        default:
            break;
        }
    }

    return 0;
}
