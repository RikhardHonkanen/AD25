/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of FlexIO UART using interrupt. For the connections look at README.md
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

static volatile bool rx_buffer_empty = true;
static volatile bool tx_buffer_full = false;
static volatile bool transmitting = false;
static volatile bool receiving = false;

void transfer_callback(FLEXIO_UART_Type *base, flexio_uart_handle_t *handle, status_t status, void *user_data)
{
	(void)base;
	(void)handle;
	(void)user_data;

	if (kStatus_FLEXIO_UART_RxIdle == status)
	{
		rx_buffer_empty = false;
		receiving = false;
	}

	if (kStatus_FLEXIO_UART_TxIdle == status)
	{
		tx_buffer_full = false;
		transmitting = false;
	}
}

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
		.timerIndex = {0, 1}	/* Flexible timers used for the shift registers. */
	};

	flexio_uart_config_t config;
	FLEXIO_UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BAUDRATE;
	config.enableUart = true;

	assert(kStatus_Success == FLEXIO_UART_Init(&uart_dev, &config, CLOCK_GetFlexioClkFreq()));

	flexio_uart_handle_t handle;
	FLEXIO_UART_TransferCreateHandle(&uart_dev, &handle, transfer_callback, NULL);

	uint8_t tx_buffer[MSGLEN], rx_buffer[MSGLEN];
	flexio_uart_transfer_t write, read;
	write.dataSize = MSGLEN;
	read.dataSize = MSGLEN;
	write.data = tx_buffer;
	read.data = rx_buffer;

	while (1)
	{
		// If RX is idle and rx_buffer is empty, start to receive data
		if ((!receiving) && rx_buffer_empty)
		{
			receiving = true;
			assert(kStatus_Success == FLEXIO_UART_TransferReceiveNonBlocking(&uart_dev, &handle, &read, NULL));
		}

		// If TX is idle and tx_buffer is full, start to send data
		if ((!transmitting) && tx_buffer_full)
		{
			transmitting = true;
			assert(kStatus_Success == FLEXIO_UART_TransferSendNonBlocking(&uart_dev, &handle, &write));
		}

		// If tx_buffer is empty and rx_buffer is full, copy rx_buffer to tx_buffer
		if ((!rx_buffer_empty) && (!tx_buffer_full))
		{
			PRINTF("Received: ");
			for (int i = 0; i < MSGLEN; i++)
			{
				PUTCHAR(rx_buffer[i]);
				tx_buffer[i] = toupper(rx_buffer[i]);
			}
			PRINTF("\r\n");

			rx_buffer_empty = true;
			tx_buffer_full = true;
		}
	}

	return 0;
}
