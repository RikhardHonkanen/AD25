/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a pwm signal using a ctimer match. Connect a LED to P0_24.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#ctimer-standard-counter-timers
 *
 * @version 0.1
 * @date 2025-10-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <assert.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_ctimer.h"
#include "fsl_debug_console.h"

#define PWM_FREQUENCY 1000U
#define PWM_DUTY_CHANNEL kCTIMER_Match_0
#define PWM_PERIOD_CHANNEL kCTIMER_Match_1

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach the clock to the timer */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_CTIMER0);

    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER0, &config);

    uint8_t duty_cycle = 0;

    // When the counter reaches the pulse period value in the match register, the PWM output toggles to end the pulse portion.
    // When the counter reaches the pwm period value in the match register, the counter resets to 0 to start of a new PWM cycle.
    assert(kStatus_Success == CTIMER_SetupPwm(CTIMER0, PWM_PERIOD_CHANNEL, PWM_DUTY_CHANNEL,
                                              duty_cycle, PWM_FREQUENCY, CLOCK_GetCTimerClkFreq(0), false));
    CTIMER_StartTimer(CTIMER0);

    while (1)
    {
        SDK_DelayAtLeastUs(25000, SystemCoreClock);

        duty_cycle = (duty_cycle + 1) % 101;

        assert(kStatus_Success == CTIMER_UpdatePwmDutycycle(CTIMER0, PWM_PERIOD_CHANNEL, PWM_DUTY_CHANNEL, duty_cycle));
    }

    return 0;
}
