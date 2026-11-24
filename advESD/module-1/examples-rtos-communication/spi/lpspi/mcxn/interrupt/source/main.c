/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of LPSPI2 in master transfer polling mode and LPSPI3 in slave interrupt mode.
		  LPSPI2 and LPSPI3 pins on the same board shall be connected together according to README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpspi-peripheral-driver
 *
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "board.h"
#include "pin_mux.h"
#include "fsl_lpspi.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 10
#define BAUDRATE 1000000

#define SLAVE LPSPI3
#define SLAVE_IRQn LP_FLEXCOMM3_IRQn
#define SLAVE_IRQ_HANDLER LP_FLEXCOMM3_IRQHandler

#define MASTER LPSPI2
#define MASTER_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(2)

void SLAVE_IRQ_HANDLER(void)
{
	static uint8_t rx_counter = 0;
	static uint8_t tx_counter = 0;
	static uint8_t buffer[MSGLEN] = {0};

	LPSPI_ClearStatusFlags(SLAVE, kLPSPI_AllStatusFlag);

	if (rx_counter < MSGLEN)
	{
		while (0 < LPSPI_GetRxFifoCount(SLAVE))
		{
			buffer[rx_counter++] = LPSPI_ReadData(SLAVE);

			if (rx_counter == MSGLEN)
			{
				break;
			}
		}
	}

	if (tx_counter < MSGLEN)
	{
		uint8_t fifo_size = LPSPI_GetTxFifoSize(SLAVE);
		while (fifo_size > LPSPI_GetTxFifoCount(SLAVE))
		{
			LPSPI_WriteData(SLAVE, toupper(buffer[tx_counter++]));

			if (tx_counter == MSGLEN)
			{
				break;
			}
		}
	}

	if ((rx_counter == MSGLEN) && (tx_counter == MSGLEN))
	{
		rx_counter = tx_counter = 0;
		LPSPI_FlushFifo(SLAVE, true, true);
	}
}

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	BOARD_InitBootPeripherals();

	// Attach FRO 12M to FLEXCOMM2
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

	// Attach FRO 12M to FLEXCOMM3
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom3Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);

	lpspi_slave_config_t slave_config;
	LPSPI_SlaveGetDefaultConfig(&slave_config);
	slave_config.whichPcs = kLPSPI_Pcs0;
	LPSPI_SlaveInit(SLAVE, &slave_config);

	LPSPI_EnableInterrupts(SLAVE, kLPSPI_RxInterruptEnable | kLPSPI_TxInterruptEnable);
	NVIC_EnableIRQ(SLAVE_IRQn);

	lpspi_master_config_t master_config;
	LPSPI_MasterGetDefaultConfig(&master_config);
	master_config.baudRate = BAUDRATE;

	LPSPI_MasterInit(MASTER, &master_config, MASTER_CLK_FREQ);

	uint8_t buffer[MSGLEN + 1] = {0U};
	lpspi_transfer_t xfer = {.dataSize = MSGLEN};

	while (1)
	{
		for (int i = 0; i < MSGLEN; i++)
		{
			buffer[i] = 'a' + (rand() % 26);
		}

		xfer.rxData = NULL;
		xfer.txData = buffer;
		assert(kStatus_Success == LPSPI_MasterTransferBlocking(MASTER, &xfer));
		PRINTF("    Sent: %s\r\n", buffer);

		SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		xfer.txData = NULL;
		xfer.rxData = buffer;
		assert(kStatus_Success == LPSPI_MasterTransferBlocking(MASTER, &xfer));
		PRINTF("Received: %s\r\n\r\n", buffer);

		SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
	}

	return 0;
}
