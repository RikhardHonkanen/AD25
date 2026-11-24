/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of LPI2C in slave mode using interrupt. For the connections look at README.md
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

static volatile bool complete = false;
static uint8_t buffer[MSGLEN + 1] = {0};

static void slave_callback(LPI2C_Type *base, lpi2c_slave_transfer_t *xfer, void *param)
{
    (void)base;
    (void)param;

    switch (xfer->event)
    {
    case kLPI2C_SlaveAddressMatchEvent: /* Address match event */
        xfer->data = NULL;
        xfer->dataSize = 0;
        break;

    case kLPI2C_SlaveTransmitEvent: /* Transmit request */
        xfer->data = buffer;
        xfer->dataSize = MSGLEN;
        for (int i = 0; i < MSGLEN; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }
        break;

    case kLPI2C_SlaveReceiveEvent: /* Receive request */
        xfer->data = buffer;
        xfer->dataSize = MSGLEN;
        break;

    case kLPI2C_SlaveCompletionEvent: /* Transfer done */
        xfer->data = NULL;
        xfer->dataSize = 0;
        complete = true;
        break;

    default:
        complete = false;
        break;
    }
}

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

    lpi2c_slave_handle_t handle;
    // Create the LPI2C handle for the non-blocking transfer
    LPI2C_SlaveTransferCreateHandle(LPI2C0, &handle, slave_callback, NULL);

    assert(kStatus_Success == LPI2C_SlaveTransferNonBlocking(LPI2C0, &handle, kLPI2C_SlaveCompletionEvent | kLPI2C_SlaveAddressMatchEvent));

    while (1)
    {
        complete = false;

        while (!complete)
        {
            ;
        }

        PRINTF("Received: %s\r\n", buffer);
    }

    return 0;
}
