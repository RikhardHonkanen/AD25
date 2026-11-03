#include "board.h"
#include <stdio.h>
#include "random.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    if(!random_init())
    {
    	printf("Failed to initialize the random number generator.\r\n");
    	while(1) { __WFI(); }
    }

    uint32_t value;

    while (1)
    {
    	SDK_DelayAtLeastUs(500000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    	if(random_get((uint8_t *)&value, sizeof(value)))
    	{
    		printf("Random Number: %010u\r\n", value);
    	}
    	else
    	{
    		printf("Failed to generate random number.\r\n");
    	}
    }

    return 0;
}
