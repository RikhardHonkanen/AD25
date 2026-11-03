
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

static volatile uint32_t systick_millis = 0;

void SysTick_Handler(void)
{
	systick_millis++;
}

int main(void)
{
    // Init board hardware
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

    uint32_t prev_millis = systick_millis;

    while(1)
    {
    	// Every 1000ms print Hello World to the terminal
    	if((systick_millis - prev_millis) >= 1000)
    	{
    		prev_millis = systick_millis;
    		printf("\r\nHello World!");
    	}
    }

    return 0 ;
}
