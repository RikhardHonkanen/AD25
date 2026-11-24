/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART polling. LPUART4 is used for the serial communication between the board and the computer.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpuart-driver
 *
 * @version 0.1
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include <assert.h>
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "clock_config.h"

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    // Attach FRO 12M to FLEXCOMM4
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    lpuart_config_t config;
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;
    assert(kStatus_Success == LPUART_Init(LPUART4, &config, CLOCK_GetFreq(kCLOCK_Fro12M)));

    uint8_t buffer[] = "UART polling echo example\r\n";
    assert(kStatus_Success == LPUART_WriteBlocking(LPUART4, buffer, sizeof(buffer) - 1));

    uint8_t chr;

    while (1)
    {
        assert(kStatus_Success == LPUART_ReadBlocking(LPUART4, &chr, sizeof(chr)));
        if (chr == '\r')
        {
            assert(kStatus_Success == LPUART_WriteBlocking(LPUART4, &chr, sizeof(chr)));
            chr = '\n';
        }
        assert(kStatus_Success == LPUART_WriteBlocking(LPUART4, &chr, sizeof(chr)));
    }

    return 0;
}
