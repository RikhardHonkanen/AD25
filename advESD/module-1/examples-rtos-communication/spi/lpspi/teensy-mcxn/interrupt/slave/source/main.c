/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of SPI interrupt in slave mode. For the connections look at README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpspi-peripheral-driver
 *
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_lpspi.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 8

void LP_FLEXCOMM3_IRQHandler(void)
{
	static uint8_t rx_counter = 0;
	static uint8_t tx_counter = 0;
	static uint8_t buffer[MSGLEN] = {0};

	LPSPI_ClearStatusFlags(LPSPI3, kLPSPI_AllStatusFlag);

	if (rx_counter < MSGLEN)
	{
		while (0 < LPSPI_GetRxFifoCount(LPSPI3))
		{
			buffer[rx_counter++] = LPSPI_ReadData(LPSPI3);

			if (rx_counter == MSGLEN)
			{
				break;
			}
		}
	}

	if (tx_counter < MSGLEN)
	{
		uint8_t fifo_size = LPSPI_GetTxFifoSize(LPSPI3);
		while (fifo_size > LPSPI_GetTxFifoCount(LPSPI3))
		{
			LPSPI_WriteData(LPSPI3, toupper(buffer[tx_counter++]));

			if (tx_counter == MSGLEN)
			{
				break;
			}
		}
	}

	if ((rx_counter == MSGLEN) && (tx_counter == MSGLEN))
	{
		rx_counter = tx_counter = 0;
		LPSPI_FlushFifo(LPSPI3, true, true);
	}
}

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	BOARD_InitBootPeripherals();

	// Attach FRO 12M to FLEXCOMM3
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom3Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);

	lpspi_slave_config_t config;
	LPSPI_SlaveGetDefaultConfig(&config);
	config.whichPcs = kLPSPI_Pcs0;
	LPSPI_SlaveInit(LPSPI3, &config);

	LPSPI_EnableInterrupts(LPSPI3, kLPSPI_RxInterruptEnable | kLPSPI_TxInterruptEnable);
	NVIC_EnableIRQ(LP_FLEXCOMM3_IRQn);

	while (1)
	{
		;
	}

	return 0;
}
