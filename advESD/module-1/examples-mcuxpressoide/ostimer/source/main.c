#include <stdio.h>
#include "board.h"
#include "fsl_cmc.h"
#include "pin_mux.h"
#include "fsl_ostimer.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define OSTIMER_FREQ 16000

// The Core Mode Controller Driver is used to enter the system in deep sleep mode
static void EnterDeepSleep(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_DeepSleepMode;

    CMC_EnterLowPowerMode(CMC0, &config);

    __WFI(); // Wait For Interrupt; i.e. the processor sleeps until an interrupt occurs
}

static void ostimer_callback(void)
{
	printf("\r\nWakeup from deep sleep mode.");
}

int main(void)
{
    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Initialize the OS timer, setting clock configuration
    CLOCK_SetupClk16KClocking(kCLOCK_Clk16KToWake);
    CLOCK_AttachClk(kCLK_16K2_to_OSTIMER);
    OSTIMER_Init(OSTIMER0);

    uint64_t timer_ticks;

    while (1)
    {
    	timer_ticks = OSTIMER_GetCurrentTimerValue(OSTIMER0);

    	// Convert the millisecond to ostimer count value
    	timer_ticks += MSEC_TO_COUNT(2000U, CLOCK_GetOstimerClkFreq());

        // Set the match value to trigger the match interrupt every 2 seconds
        if (kStatus_Success == OSTIMER_SetMatchValue(OSTIMER0, timer_ticks, ostimer_callback))
        {
        	EnterDeepSleep(); // Enter deep sleep mode
        }
        else
        {
        	printf("SetMatchInterruptTime failed: set time has already expired! \r\n");
        }
    }

    return 0 ;
}
