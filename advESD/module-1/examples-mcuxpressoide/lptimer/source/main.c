#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_lptmr.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

static volatile bool completed = false;

void LPTMR0_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    completed = true;
}

int main(void)
{
    // Init board hardware.
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Configures the 16 kHz clock source
    CLOCK_SetupClk16KClocking(kCLOCK_Clk16KToVsys);

    lptmr_config_t config;
    LPTMR_GetDefaultConfig(&config);
    LPTMR_Init(LPTMR0, &config);

    // Set timer period
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(1000000, CLOCK_GetFreq(kCLOCK_Clk16K1)));

    // Enable timer interrupt
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);

    // Enable the interrupt in the NVIC
    NVIC_EnableIRQ(LPTMR0_IRQn);

    // Start the timer
    LPTMR_StartTimer(LPTMR0);

    while(1)
    {
        if (completed)
        {
        	completed = false;
        	printf("\r\nHello World!");
        }
    }

    return 0 ;
}
