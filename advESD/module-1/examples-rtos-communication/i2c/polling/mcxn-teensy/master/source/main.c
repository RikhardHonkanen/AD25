/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of LPI2C in master mode using polling. For the connections look at README.md
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#lpi2c-master-driver
 *
 * @version 0.1
 * @date 2025-10-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include <assert.h>
#include "pin_mux.h"
#include "fsl_lpi2c.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define SLAVE_ADDRESS 0x55
#define LPI2C_BAUDRATE 400000U

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	BOARD_InitBootPeripherals();

	// Attach 12 MHz clock to FLEXCOMM0
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

	lpi2c_master_config_t config = {0};
	LPI2C_MasterGetDefaultConfig(&config);
	config.baudRate_Hz = LPI2C_BAUDRATE;

	LPI2C_MasterInit(LPI2C0, &config, CLOCK_GetLPFlexCommClkFreq(0));

	uint8_t buffer[MSGLEN + 1] = {0};
	lpi2c_master_transfer_t xfer = {0};
	xfer.data = buffer;
	xfer.dataSize = MSGLEN;
	xfer.slaveAddress = SLAVE_ADDRESS;
	xfer.flags = kLPI2C_TransferDefaultFlag;

	while (1)
	{
		SDK_DelayAtLeastUs(500000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		for (int i = 0; i < MSGLEN; i++)
		{
			buffer[i] = 'a' + (rand() % 26);
		}

		xfer.direction = kLPI2C_Write;
		if (kStatus_Success == LPI2C_MasterTransferBlocking(LPI2C0, &xfer))
		{
			printf("    Sent: %s\r\n", buffer);

			xfer.direction = kLPI2C_Read;
			if (kStatus_Success == LPI2C_MasterTransferBlocking(LPI2C0, &xfer))
			{
				printf("Received: %s\r\n\r\n", buffer);
			}
			else
			{
				printf("Failed\r\n");
			}
		}
		else
		{
			printf("Failed\r\n");
		}
	}

	return 0;
}
