#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_ctimer.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

static volatile bool match_flag = false;

void CTIMER0_IRQHandler(void)
{
	/* Check for match on channel 0 */
    if (CTIMER_GetStatusFlags(CTIMER0) & kCTIMER_Match0Flag)
    {
        CTIMER_ClearStatusFlags(CTIMER0, kCTIMER_Match0Flag);
        match_flag = true;
    }
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach the clock to the timer */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1U);
    CLOCK_AttachClk(kFRO12M_to_CTIMER0);

    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER0, &config);

    /* Match Configuration */
    ctimer_match_config_t match_config = {
    		.enableCounterReset = true,
			.enableCounterStop  = false,
			.enableInterrupt    = true,
			.matchValue         = CLOCK_GetFreq(kCLOCK_Fro12M) / 2,
    };

    CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &match_config);

    NVIC_EnableIRQ(CTIMER0_IRQn); /* Enable interrupt in the NVIC */

    CTIMER_StartTimer(CTIMER0); /* Start timer */

    while(1)
    {
    	if(match_flag)
    	{
    		match_flag = false;
    		printf("\r\nHello World!");
    	}
    }

    return 0 ;
}
