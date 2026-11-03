/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of the INTM (Interrupt Monitor) module.
 *        The INTM (Interrupt Monitor) module is used to monitor the response times of up to four interrupts simultaneously,
 *        primarily to detect timeouts between an interrupt request and its servicing by the CPU.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#intm-interrupt-monitor-driver
 *
 * @version 0.1
 * @date 2025-10-12
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_intm.h"
#include "fsl_debug_console.h"

#define MIN_US_DELAY 8
#define MAX_US_DELAY 10
#define MAX_US_TIME MAX_US_DELAY

static volatile bool button_press = false;
static volatile bool timeout_status = false;

void GPIO00_IRQHandler(void)
{
    // A delay to let intm timeout occur
    SDK_DelayAtLeastUs(MIN_US_DELAY + (rand() % (MAX_US_DELAY - MIN_US_DELAY)), SystemCoreClock);

    INTM_AckIrq(INTM0, BOARD_SW3_IRQ); // Acknowledge the interrupt

    timeout_status = INTM_GetStatusFlags(INTM0, kINTM_Monitor1); // Get interrupt monitor channel status

    INTM_ClearTimeCount(INTM0, kINTM_Monitor1); // Clear time out

    GPIO_GpioClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN); // Clear the interrupt flag

    button_press = true;
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    gpio_pin_config_t sw3_config = {kGPIO_DigitalInput};
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw3_config);

    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, kGPIO_InterruptFallingEdge);
    NVIC_EnableIRQ(BOARD_SW3_IRQ);

    intm_config_t intm_config[INTM_MON_COUNT];
    INTM_GetDefaultConfig(intm_config);

    intm_config[kINTM_Monitor1].intm->irqnumber = BOARD_SW3_IRQ;
    intm_config[kINTM_Monitor1].intm->maxtimer = USEC_TO_COUNT(MAX_US_TIME, SystemCoreClock);
    intm_config->enable = true;

    INTM_Init(INTM0, intm_config);

    printf("\r\n");

    while (1)
    {
        printf("Press %s to trigger interrupt: ", BOARD_SW3_NAME);

        button_press = false;
        while (!button_press)
        {
            ;
        }

        if (timeout_status)
        {
            printf("INTM timeout.\r\n");
        }
        else
        {
            printf("INTM no timeout.\r\n");
        }
    }

    return 0;
}
