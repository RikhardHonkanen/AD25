
#include <stdio.h>
#include <assert.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_irtc.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

static volatile bool alarm_flag = false;
static volatile bool second_flag = false;

void RTC_IRQHandler(void)
{
    uint32_t flags = IRTC_GetStatusFlags(RTC);

	IRTC_SetWriteProtection(RTC, false); // Unlock to allow register write operation
    IRTC_ClearStatusFlags(RTC, flags); // Clear all flags

	alarm_flag = (0U != (flags & kIRTC_AlarmFlag));
	second_flag = (0U != (flags & kIRTC_1hzFlag));
}

int main(void) {

    // Init board hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Setup the 16 kHz clock source
    CLOCK_SetupClk16KClocking(kCLOCK_Clk16KToAll);

    irtc_config_t config;
    IRTC_GetDefaultConfig(&config);
    assert(kStatus_Fail != IRTC_Init(RTC, &config));

    IRTC_EnableInterrupts(RTC, kIRTC_1hzInterruptEnable | kIRTC_AlarmInterruptEnable); // Enable RTC interrupts

    NVIC_EnableIRQ(RTC_IRQn); // Enable the interrupt in the NVIC

    printf("RTC Example Start ...\r\n");

    irtc_datetime_t datetime = {
    		.year    = 2025,
			.month   = 1,
			.day     = 21,
			.hour    = 18,
			.minute  = 55,
			.second  = 30
    };

    IRTC_SetDatetime(RTC, &datetime);

    datetime.second = 35;
    IRTC_SetAlarm(RTC, &datetime);

    while(!alarm_flag)
    {
    	;
    }

    IRTC_GetDatetime(RTC, &datetime);
    printf("\r\nAlarm Time: %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n",
    		datetime.year, datetime.month, datetime.day,
    		datetime.hour, datetime.minute, datetime.second);

    while(1)
    {
    	if(second_flag)
    	{
    		second_flag = false;
        	IRTC_GetDatetime(RTC, &datetime);
        	printf("Datetime: %04d-%02d-%02d %02d:%02d:%02d\r\n",
        	    		datetime.year, datetime.month, datetime.day,
        	    		datetime.hour, datetime.minute, datetime.second);
    	}
    }

    return 0 ;
}
