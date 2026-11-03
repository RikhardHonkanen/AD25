/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of toggling an output using ctimer match interrupt.
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
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_Fro12M)

static ctimer_match_config_t match_config;

static void ctimer_match_callback(uint32_t flags)
{
	CTIMER_ClearStatusFlags(CTIMER3, flags);

    static uint32_t blink_rate = 0;
    static uint32_t blink_times = 0;

    if (++blink_times > 8)
    {
    	blink_times = 0;
        match_config.matchValue >>= 1;

        if(++blink_rate > 4)
        {
        	blink_rate = 0;
            match_config.matchValue = CTIMER_CLK_FREQ / 2;
        }

        CTIMER_SetupMatch(CTIMER3, kCTIMER_Match_1, &match_config);
    }
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    /* Attach the clock to the timer */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer3Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_CTIMER3);

    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER3, &config);

    match_config.enableCounterReset = true;
    match_config.enableCounterStop = false;
    match_config.matchValue = CTIMER_CLK_FREQ / 2;
    match_config.outControl = kCTIMER_Output_Toggle;
    match_config.outPinInitState = true;
    match_config.enableInterrupt = true;
    CTIMER_SetupMatch(CTIMER3, kCTIMER_Match_1, &match_config);

    ctimer_callback_t ctimer_callback = ctimer_match_callback;
    CTIMER_RegisterCallBack(CTIMER3, &ctimer_callback, kCTIMER_SingleCallback);

    CTIMER_StartTimer(CTIMER3);

    while (1)
    {
        __WFI();
    }

    return 0;
}
