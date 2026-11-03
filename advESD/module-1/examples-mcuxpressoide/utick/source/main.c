#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_utick.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define INTERVAL 500000 // 500 millisecond

static volatile bool expired;

static void UTickCallback(void)
{
	expired = true;
}

static void UTickDelay(uint32_t usec)
{
    // Set the UTICK timer to wake up the device from reduced power mode
    UTICK_SetTick(UTICK0, kUTICK_Onetime, usec, UTickCallback);

    while (!expired)
    {
    	;
    }

    expired = false;
}

int main(void)
{
    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Initialize UTICK0
    UTICK_Init(UTICK0);

    while (1)
    {
    	printf("\r\nHello World!");
        UTickDelay(INTERVAL);
    }

    return 0 ;
}
