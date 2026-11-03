
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

static volatile uint32_t systick_counter = 0;

void SysTick_Handler(void)
{
    if (systick_counter != 0U)
    {
    	systick_counter--;
    }
}

void systick_Delay(uint32_t ms)
{
	systick_counter = ms;

    while (systick_counter != 0U)
    {
    	;
    }
}


int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Set systick reload value to generate 1ms interrupt
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        	;
        }
    }

    while(1)
    {
    	systick_Delay(1000);
    	printf("\r\nHello World!");
    }

    return 0 ;
}
