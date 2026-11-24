/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART using interrupt. LPUART4 is used for the serial communication between the board and the computer.
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
#include <string.h>
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "clock_config.h"

static uint8_t buffer[32];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;
static volatile uint8_t count = 0;

static void cbuffer_write(uint8_t data)
{
	buffer[tail] = data;
	tail = (tail + 1) % sizeof(buffer);

	if (count == sizeof(buffer))
	{
		head = (head + 1) % sizeof(buffer);
	}
	else
	{
		count++;
	}
}

static inline bool cbuffer_isempty(void)
{
	return (count == 0);
}

static uint8_t cbuffer_read(void)
{
	uint8_t data = buffer[head];

	if (count > 0)
	{
		head = (head + 1) % sizeof(buffer);
		count--;
	}

	return data;
}

void LP_FLEXCOMM4_IRQHandler(void)
{
	uint32_t flags = LPUART_GetStatusFlags(LPUART4);

	LPUART_ClearStatusFlags(LPUART4, kLPUART_RxDataRegFullFlag);

	if (0 != (kLPUART_RxDataRegFullFlag & flags))
	{
		uint8_t chr = LPUART_ReadByte(LPUART4);

		cbuffer_write(chr);

		if (chr == '\r')
		{
			cbuffer_write('\n');
		}
	}
}

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

	// Attach FRO 12M clock to FLEXCOMM4
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

	lpuart_config_t config;
	LPUART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;
	assert(kStatus_Success == LPUART_Init(LPUART4, &config, CLOCK_GetFreq(kCLOCK_Fro12M)));

	strcpy((char *)buffer, "UART polling echo example\r\n");
	assert(kStatus_Success == LPUART_WriteBlocking(LPUART4, buffer, strlen((const char *)buffer)));

	// Enable RX and TX interrupt
	LPUART_EnableInterrupts(LPUART4, kLPUART_RxDataRegFullInterruptEnable);
	NVIC_EnableIRQ(LP_FLEXCOMM4_IRQn);

	while (1)
	{
		if (!cbuffer_isempty() && (0 != (kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(LPUART4))))
		{
			LPUART_WriteByte(LPUART4, cbuffer_read());
		}
	}

	return 0;
}
