/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of LPI2C in master mode using interrupt. For the connections look at README.md
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
#include <assert.h>
#include <stdlib.h>
#include "pin_mux.h"
#include "fsl_lpi2c.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define MSGLEN 8
#define SLAVE_ADDRESS 0x55
#define LPI2C_BAUDRATE 400000U

static volatile bool complete = false;
static uint8_t buffer[MSGLEN + 1] = {0};

static void master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *user_data)
{
	(void)base;
	(void)handle;
	(void)user_data;

	if (status == kStatus_Success)
	{
		complete = true;
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

	lpi2c_master_config_t config = {0};
	LPI2C_MasterGetDefaultConfig(&config);
	config.baudRate_Hz = LPI2C_BAUDRATE;

	LPI2C_MasterInit(LPI2C0, &config, CLOCK_GetLPFlexCommClkFreq(0));

	lpi2c_master_handle_t handle;
	// Create the LPI2C handle for the non-blocking transfer
	LPI2C_MasterTransferCreateHandle(LPI2C0, &handle, master_callback, NULL);

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

		complete = false;
		xfer.direction = kLPI2C_Write;
		assert(kStatus_Success == LPI2C_MasterTransferNonBlocking(LPI2C0, &handle, &xfer));

		while (!complete)
		{
			;
		}

		printf("    Sent: %s\r\n", buffer);

		complete = false;
		xfer.direction = kLPI2C_Read;
		assert(kStatus_Success == LPI2C_MasterTransferNonBlocking(LPI2C0, &handle, &xfer));

		while (!complete)
		{
			;
		}

		printf("Received: %s\r\n\r\n", buffer);
	}

	return 0;
}
