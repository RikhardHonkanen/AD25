/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using WWDT0. Look at:
 *        https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#wwdt-windowed-watchdog-timer-driver
 *
 * @version 0.1
 * @date 2025-11-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_cmc.h"
#include "fsl_wwdt.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

__attribute__((section(".noinit"))) uint32_t wdog_reset_count; // Place the counter in the noinit section

void WWDT0_IRQHandler(void)
{
	// Handle warning interrupt
	if (kWWDT_WarningFlag & WWDT_GetStatusFlags(WWDT0))
	{
		WWDT_ClearStatusFlags(WWDT0, kWWDT_WarningFlag);

		// User code. Urgent code before timeout reset
		printf("WDOG didn't feed before the timeout!\r\n");
	}
}

int main(void)
{
	// Init board hardware
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	CLOCK_SetClkDiv(kCLOCK_DivWdt0Clk, 1U); // Set clock divider for WWDT clock source
	CLOCK_EnableClock(kCLOCK_Wwdt0);		// Enable the watchdog clock whose frequency is 1MHz
	NVIC_EnableIRQ(WWDT0_IRQn);

	if (kCMC_WindowedWatchdog0Reset & CMC_GetSystemResetStatus(CMC0))
	{
		wdog_reset_count++;
		printf("WDOG Reset (%lu)\r\n", wdog_reset_count);
	}
	else
	{
		wdog_reset_count = 0;
	}

	wwdt_config_t config;
	WWDT_GetDefaultConfig(&config);

	const uint32_t WDT_CLK_FREQ = CLOCK_GetWdtClkFreq(0); // Watchdog0 clock frequency(Hz)
	const uint32_t WDT_CTR_FREQ = WDT_CLK_FREQ / 4;		  /* The input clock from the watchdog oscillator (WDT_OSC) is
															 internally divided by 4 before it drives the counter */

	/*
	 A match of the WWDT counter to warningValue occurs when the lower 10 bits of the timer counter
	 have the same value as the lower 10 bits of warningValue, and the remaining upper bits of the
	 counter are all 0. This gives a maximum time of 1023 watchdog timer counts (4096 watchdog clocks)
	 for the interrupt to occur before a watchdog reset. For no warning set warningValue to 0.
	*/
	config.enableWatchdogReset = true;					  // Configure WWDT to reset on timeout
	config.clockFreq_Hz = WDT_CLK_FREQ;					  // Setup watchdog0 clock frequency(Hz)
	config.warningValue = MSEC_TO_COUNT(4, WDT_CTR_FREQ); // Equal to 1000 counter ticks and 4000 clock ticks

	switch (wdog_reset_count)
	{
	case 0:
		printf("\r\n--- Start Of The Example ---\r\n");
		printf("\r\n--- Non-Window Mode Refresh Test Started ---\r\n");
		config.timeoutValue = MSEC_TO_COUNT(2000, WDT_CTR_FREQ);
		WWDT_Init(WWDT0, &config);

		while (WWDT0->TV > WWDT0->TC / 2)
		{
			;
		}

		WWDT_Refresh(WWDT0);
		printf("WDOG Refreshed!\r\n");

		while (WWDT0->TV > 0)
		{
			;
		}

		printf("\r\n--- WDOG Reset Failed ---\r\n"); // If reset is not triggered, you will see this log
		break;

	case 1:
		printf("--- Non-Window Mode Refresh Test Done ---\r\n");
		printf("\r\n--- Window Mode Refresh Test Started ---\r\n");
		config.timeoutValue = MSEC_TO_COUNT(3000, WDT_CTR_FREQ); // TC (Timer Constant) is used to load the counter with
		config.windowValue = MSEC_TO_COUNT(2000, WDT_CTR_FREQ);	 // The Window Compare Value
		WWDT_Init(WWDT0, &config);

		//      Safe Refresh Zone (0 < TV < WV)
		// 0 ------------------------------------- WV ------------------- TC

		while (WWDT0->TV > (WWDT0->WINDOW - 100))
		{
			;
		}

		WWDT_Refresh(WWDT0);
		printf("WDOG Refreshed!\r\n");

		while (WWDT0->TV > (WWDT0->WINDOW + 100))
		{
			;
		}

		WWDT_Refresh(WWDT0);
		printf("WDOG Refreshed!\r\n"); // If reset is not triggered, you will see this log
		break;

	default:
		printf("--- Window Mode Refresh Test Done ---\r\n");
		printf("\r\nEnd Of The Example!\r\n");
		break;
	}

	while (1)
	{
		;
	}

	return 0;
}
