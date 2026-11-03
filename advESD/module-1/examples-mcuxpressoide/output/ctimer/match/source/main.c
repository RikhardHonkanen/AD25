/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of toggling an output using ctimer match.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#ctimer-standard-counter-timers
 *
 * @version 0.1
 * @date 2025-10-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "board.h"
#include "pin_mux.h"
#include "fsl_ctimer.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

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

    ctimer_match_config_t match_config;
    match_config.enableCounterReset = true;
    match_config.enableCounterStop = false;
    match_config.matchValue = CLOCK_GetFreq(kCLOCK_Fro12M) / 2;
    match_config.outControl = kCTIMER_Output_Toggle;
    match_config.outPinInitState = true;
    match_config.enableInterrupt = false;
    CTIMER_SetupMatch(CTIMER3, kCTIMER_Match_1, &match_config);

    CTIMER_StartTimer(CTIMER3);

    while (1)
    {
        __WFI();
    }

    return 0;
}
