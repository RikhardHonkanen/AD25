/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of SPI in slave mode.
 *
 * @version 0.1
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include <fsl_lpspi.h>
#include <zephyr/kernel.h>

#define MSG_SIZE 8

int main(void)
{
	lpspi_slave_config_t config;
	LPSPI_SlaveGetDefaultConfig(&config);
	LPSPI_SlaveInit(LPSPI3, &config);

	uint8_t rx_counter = 0;
	uint8_t tx_counter = 0;
	uint8_t buffer[MSG_SIZE] = {0};
	uint8_t fifo_size = LPSPI_GetTxFifoSize(LPSPI3);

	while (1)
	{
		if (rx_counter < MSG_SIZE)
		{
			while (0 < LPSPI_GetRxFifoCount(LPSPI3))
			{
				buffer[rx_counter++] = LPSPI_ReadData(LPSPI3);

				if (rx_counter == MSG_SIZE)
				{
					break;
				}
			}
		}

		if (tx_counter < MSG_SIZE)
		{
			while (fifo_size > LPSPI_GetTxFifoCount(LPSPI3))
			{
				LPSPI_WriteData(LPSPI3, toupper(buffer[tx_counter++]));

				if (tx_counter == MSG_SIZE)
				{
					break;
				}
			}
		}

		if ((rx_counter == MSG_SIZE) && (tx_counter == MSG_SIZE))
		{
			rx_counter = tx_counter = 0;
			LPSPI_FlushFifo(LPSPI3, true, true);
		}
	}

	return 0;
}
