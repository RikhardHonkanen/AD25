/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of FlexIO UART using polling. For the connections look at README.md
 *        A FlexIO module allows the software driver to emulate different protocols by using its shifters, timers, and pins.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#flexio-uart-driver
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
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_flexio_uart.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define BAUDRATE 115200
#define FLEXIO_UART_RX_PIN 0U
#define FLEXIO_UART_TX_PIN 1U

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    // Attach the HF clock to FLEXIO0
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO);

    FLEXIO_UART_Type uart_dev = {
        .flexioBase = FLEXIO0,
        .TxPinIndex = FLEXIO_UART_TX_PIN,
        .RxPinIndex = FLEXIO_UART_RX_PIN,
        .shifterIndex = {0, 1}, /* Two 32-bit shift registers are used to transmit and receive data.
                                   Shifter 0 is used to transmit data and shifter 1 to receive data. */
        .timerIndex = {0, 1}    /* Flexible timers used for the shift registers. */
    };

    flexio_uart_config_t config;
    FLEXIO_UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BAUDRATE;
    config.enableUart = true;

    assert(kStatus_Success == FLEXIO_UART_Init(&uart_dev, &config, CLOCK_GetFlexioClkFreq()));

    uint8_t buffer[MSGLEN] = {0};

    while (1)
    {
        FLEXIO_UART_ReadBlocking(&uart_dev, buffer, MSGLEN);

        PRINTF("Received: ");
        for (int i = 0; i < MSGLEN; i++)
        {
            PUTCHAR(buffer[i]);
            buffer[i] = toupper(buffer[i]);
        }
        PRINTF("\r\n");

        FLEXIO_UART_WriteBlocking(&uart_dev, buffer, MSGLEN);
    }

    return 0;
}
