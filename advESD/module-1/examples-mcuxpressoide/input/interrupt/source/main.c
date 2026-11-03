/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of digital input interrupt.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#port-port-control-and-interrupts
 *                 https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#gpio-driver
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

#define BUTTON_PORT PORT0
#define BUTTON_PORT_CLOCK kCLOCK_Port0

#define BUTTON_GPIO BOARD_SW3_GPIO
#define BUTTON_GPIO_CLOCK kCLOCK_Gpio0
#define BUTTON_GPIO_IRQ_NUM GPIO00_IRQn

#define BUTTON_PIN BOARD_SW3_GPIO_PIN

static volatile bool pressed = false;

void GPIO00_IRQHandler(void)
{
    __disable_irq(); // Disable all interrupts

    if (0 != (GPIO_GpioGetInterruptFlags(BUTTON_GPIO) & (1U << BUTTON_PIN)))
    {

        GPIO_GpioClearInterruptFlags(BUTTON_GPIO, 1U << BUTTON_PIN); // Clear the interrupt flag

        pressed = true;
    }

    __enable_irq(); // Enable all interrupts
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Configure the button pin as GPIO */
    CLOCK_EnableClock(BUTTON_PORT_CLOCK); // Enables port clock

    const port_pin_config_t port_pin_config = {
        .pullSelect = kPORT_PullUp,                        /* Internal pull-up resistor is enabled */
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

    PORT_SetPinConfig(BUTTON_PORT, BUTTON_PIN, &port_pin_config);

    CLOCK_EnableClock(BUTTON_GPIO_CLOCK); // Enables the gpio clock

    /* Config the button pin as a digital input pin */
    const gpio_pin_config_t gpio_pin_config = {kGPIO_DigitalInput};

    GPIO_PinInit(BUTTON_GPIO, BUTTON_PIN, &gpio_pin_config);

    GPIO_SetPinInterruptConfig(BUTTON_GPIO, BUTTON_PIN, kGPIO_InterruptFallingEdge);

    EnableIRQ(BUTTON_GPIO_IRQ_NUM); // Enable the interrupt in the NVIC

    printf("\r\nPress SW3 on the board\r\n");

    uint32_t counter = 0;

    while (1)
    {
        if (pressed)
        {
            pressed = false;
            printf("%lu) Pressed\r\n", ++counter);
        }
    }

    return 0;
}
