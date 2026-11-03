#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_mrt.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define ONE_SHOT_US 250000U
#define REPEATED_US 500000U

void MRT0_IRQHandler(void)
{
	if (kMRT_TimerInterruptFlag & MRT_GetStatusFlags(MRT0, kMRT_Channel_0))
	{
		MRT_ClearStatusFlags(MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag); // Clear interrupt flag

		printf("One Shot Timer Interrupt\r\n");
	}

	if (kMRT_TimerInterruptFlag & MRT_GetStatusFlags(MRT0, kMRT_Channel_1))
	{
		MRT_ClearStatusFlags(MRT0, kMRT_Channel_1, kMRT_TimerInterruptFlag); // Clear interrupt flag

		printf("Repeated Timer Interrupt\r\n");
	}
}

int main(void)
{
    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    mrt_config_t config;
    MRT_GetDefaultConfig(&config);
    MRT_Init(MRT0, &config);

    MRT_SetupChannelMode(MRT0, kMRT_Channel_0, kMRT_OneShotMode); // Setup Channel 0 to be one-shot
    MRT_EnableInterrupts(MRT0, kMRT_Channel_0, kMRT_TimerInterruptEnable); // Enable timer interrupts for channel 0

    MRT_SetupChannelMode(MRT0, kMRT_Channel_1, kMRT_RepeatMode); // Setup Channel 1 to be repeated
    MRT_EnableInterrupts(MRT0, kMRT_Channel_1, kMRT_TimerInterruptEnable); // Enable timer interrupts for channel 1

    NVIC_EnableIRQ(MRT0_IRQn); // Enable the interrupt in the NVIC

    printf("Press any key to start the one-shot timer ...\r\n");
    (void)getchar();

    const uint32_t mrt_clock_freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    uint32_t period_ticks = USEC_TO_COUNT(ONE_SHOT_US, mrt_clock_freq);

    // The timer period in units of ticks shall be max. 0xFFFFFF
    if (period_ticks > MRT_CHANNEL_INTVAL_IVALUE_MASK)
    {
    	period_ticks = MRT_CHANNEL_INTVAL_IVALUE_MASK;
    }

    MRT_StartTimer(MRT0, kMRT_Channel_0, period_ticks);

    SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    printf("\r\nPress any key to start the periodic timer ...\r\n");
    (void)getchar();

    period_ticks = USEC_TO_COUNT(REPEATED_US, mrt_clock_freq);

    // The timer period in units of ticks shall be max. 0xFFFFFF
    if (period_ticks > MRT_CHANNEL_INTVAL_IVALUE_MASK)
    {
    	period_ticks = MRT_CHANNEL_INTVAL_IVALUE_MASK;
    }

    MRT_StartTimer(MRT0, kMRT_Channel_1, period_ticks);

    while(1)
    {
    	__WFI();
    }

    return 0 ;
}
