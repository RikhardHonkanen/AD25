/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of LPI2C in slave mode using polling. For the connections look at README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpi2c-slave-driver
 *
 * @version 0.1
 * @date 2025-10-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include "board.h"
#include <assert.h>
#include "pin_mux.h"
#include "fsl_lpi2c.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define ADDRESS 0x55

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    // Attach 12 MHz clock to FLEXCOMM0
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

    lpi2c_slave_config_t config = {0};
    LPI2C_SlaveGetDefaultConfig(&config);
    config.address0 = ADDRESS;

    LPI2C_SlaveInit(LPI2C0, &config, CLOCK_GetLPFlexCommClkFreq(0));

    uint8_t buffer[MSGLEN + 1] = {0};

    while (1)
    {
        // Wait until being targeted by the master
        while (0 == (kLPI2C_SlaveAddressValidFlag & LPI2C_SlaveGetStatusFlags(LPI2C0)))
        {
            ;
        }
        LPI2C_SlaveGetReceivedAddress(LPI2C0); // Clear being visited flag

        assert(kStatus_Success == LPI2C_SlaveReceive(LPI2C0, buffer, MSGLEN, NULL));
        PRINTF("Received: %s\r\n", buffer);

        for (int i = 0; i < MSGLEN; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        // Wait until being targeted by the master
        while (0 == (kLPI2C_SlaveAddressValidFlag & LPI2C_SlaveGetStatusFlags(LPI2C0)))
        {
            ;
        }
        LPI2C_SlaveGetReceivedAddress(LPI2C0); // Clear being visited flag

        assert(kStatus_Success == LPI2C_SlaveSend(LPI2C0, buffer, MSGLEN, NULL));
    }

    return 0;
}
