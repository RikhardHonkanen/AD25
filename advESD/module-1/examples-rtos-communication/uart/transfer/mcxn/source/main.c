/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART transfer. For connections look at README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpuart-driver
 *
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define BAUDRATE 115200

static volatile bool rx_buffer_empty = true;
static volatile bool tx_buffer_full = false;
static volatile bool transmitting = false;
static volatile bool receiving = false;

static void transfer_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *user_data)
{
	(void)base;
	(void)handle;
	(void)user_data;

	if (kStatus_LPUART_RxIdle == status)
	{
		rx_buffer_empty = false;
		receiving = false;
	}

	if (kStatus_LPUART_TxIdle == status)
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

	// Attach FRO 12M to FLEXCOMM0
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

	lpuart_config_t config;
	LPUART_GetDefaultConfig(&config);
	config.baudRate_Bps = BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;
	assert(kStatus_Success == LPUART_Init(LPUART0, &config, CLOCK_GetFreq(kCLOCK_Fro12M)));

	lpuart_handle_t handle;
	LPUART_TransferCreateHandle(LPUART0, &handle, transfer_callback, NULL);

	uint8_t tx_buffer[MSGLEN], rx_buffer[MSGLEN];
	lpuart_transfer_t write, read;
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
			assert(kStatus_Success == LPUART_TransferReceiveNonBlocking(LPUART0, &handle, &read, NULL));
		}

		// If TX is idle and tx_buffer is full, start to send data
		if ((!transmitting) && tx_buffer_full)
		{
			transmitting = true;
			assert(kStatus_Success == LPUART_TransferSendNonBlocking(LPUART0, &handle, &write));
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
