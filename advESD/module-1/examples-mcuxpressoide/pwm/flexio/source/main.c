/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of generating a PWM signal using FLEXIO0. Connect a LED to P1_17.
 *        The FlexIO module is a highly configurable peripheral designed to emulate various communication and
 *        timing protocols. It provides a flexible, programmable interface to generate or process signals that
 *        standard peripherals (e.g., UART, SPI, I2C, PWM) may not handle efficiently or at all.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#flexio-driver
 *
 * @version 0.1
 * @date 2025-10-12
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <assert.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_flexio.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define FLEXIO_PWM_PIN 25U      /* Select FLEXIO0_D25 which is routed to P1_17 as PWM output */
#define FLEXIO_TIMER_CHANNEL 0U /* Flexio timer0 used */

#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
#define FLEXIO_PWM_FREQUENCY (FLEXIO_CLOCK_FREQUENCY >> 8U)

static void flexio_pwm_set(uint32_t duty)
{
    assert(duty <= 100);

    uint32_t period_value = 0; /* Number of clock cycles in one period */
    uint32_t lower_value = 0;  /* Number of clock cycles in high logic state in one period */
    uint32_t upper_value = 0;  /* Number of clock cycles in low logic state in one period */

    /* Calculate timer lower and upper values of the timer compare register */
    period_value = FLEXIO_CLOCK_FREQUENCY / FLEXIO_PWM_FREQUENCY;
    lower_value = (period_value * duty) / 100; // Calculate lower value, the high period of the pwm output
    upper_value = period_value - lower_value;  // Calculate upper value, the low period of the pwm output

    flexio_timer_config_t config = {0};
    config.pinSelect = FLEXIO_PWM_PIN;
    config.pinConfig = kFLEXIO_PinConfigOutput;
    config.timerCompare = ((upper_value << 8U) | (lower_value));

    if (duty == 100)
    {
        config.pinPolarity = kFLEXIO_PinActiveLow;
    }
    else if (duty == 0)
    {
        config.pinPolarity = kFLEXIO_PinActiveHigh;
    }
    else
    {
        config.timerMode = kFLEXIO_TimerModeDual8BitPWM; // Set Timer mode to kFLEXIO_TimerModeDual8BitPWM
    }

    FLEXIO_SetTimerConfig(FLEXIO0, FLEXIO_TIMER_CHANNEL, &config);
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1U);
    CLOCK_AttachClk(kFRO12M_to_FLEXIO);

    flexio_config_t flexio_config;
    FLEXIO_GetDefaultConfig(&flexio_config);
    FLEXIO_Init(FLEXIO0, &flexio_config);

    int chr;
    uint32_t duty_cycle = 0;

    flexio_pwm_set(duty_cycle);

    while (1)
    {
        chr = 0;
        duty_cycle = 0;

        printf("\r\nEnter duty cycle in the range of [0, 100]: ");

        while (chr != '\r')
        {
            chr = getchar();

            if ((chr >= '0') && (chr <= '9'))
            {
                putchar(chr);
                duty_cycle = duty_cycle * 10 + (chr - '0');
            }
        }

        if (duty_cycle <= 100)
        {
            flexio_pwm_set(duty_cycle);
        }
        else
        {
            printf(" -> Invalid");
        }
    }

    return 0;
}
