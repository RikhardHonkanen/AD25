/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of measuring pulse width of an input signal using a ctimer interrupt in capture mode.
 *        Build and run the program, open terminal and press SW3 on the board to measure click time.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#ctimer-standard-counter-timers
 *
 * @version 0.1
 * @date 2025-10-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_ctimer.h"
#include "fsl_debug_console.h"

static volatile bool measured = false;
static volatile uint32_t measured_value = 0;

static void ctimer_capture_callback(uint32_t flags)
{
    static bool falling = true;
    static uint32_t falling_count = 0;

    CTIMER_ClearStatusFlags(CTIMER0, flags);

    if (falling)
    {
        falling_count = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_0);
    }
    else
    {
        measured_value = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_0) - falling_count;
        measured = true;
    }

    falling = !falling;
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach the clock to the timer */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kPLL0_to_CTIMER0);

    ctimer_config_t ctimer_config;
    CTIMER_GetDefaultConfig(&ctimer_config);
    CTIMER_Init(CTIMER0, &ctimer_config);

    CTIMER_SetupCapture(CTIMER0, kCTIMER_Capture_0, kCTIMER_Capture_BothEdge, true);

    ctimer_callback_t ctimer_callback = ctimer_capture_callback;
    CTIMER_RegisterCallBack(CTIMER0, &ctimer_callback, kCTIMER_SingleCallback);

    CTIMER_StartTimer(CTIMER0);

    float ctimer_ticks_ms = CLOCK_GetFreq(kCLOCK_Pll0Out) / 1000;

    printf("\r\nPress SW3 on the board to measure click time.\r\n");

    while (1)
    {
        if (measured)
        {
            measured = false;
            printf("Measured Time: %lums\r\n", (uint32_t)(measured_value / ctimer_ticks_ms));
        }
    }

    return 0;
}
