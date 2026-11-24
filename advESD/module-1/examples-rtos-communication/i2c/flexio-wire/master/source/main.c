/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of FlexIO I2C in master mode. For the connections look at README.md
 *        A FlexIO module allows the software driver to emulate different protocols by using its shifters, timers, and pins.
 *        Look at: https://mcuxpresso.nxp.com/mcuxsdk/latest/api/devices/MCXN236/index.html#flexio-i2c-master-driver
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
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_flexio_i2c_master.h"

#define MSGLEN 8
#define SLAVE_ADDRESS 0x55
#define FLEXIO_I2C_SDA_PIN 0U
#define FLEXIO_I2C_SCL_PIN 1U
#define FLEXIO_I2C_BAUDRATE 400000

static volatile bool complete = false;

static void master_callback(FLEXIO_I2C_Type *base, flexio_i2c_master_handle_t *handle, status_t status, void *user_data)
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

	// Attach the HF clock to FLEXIO0
	CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
	CLOCK_AttachClk(kFRO_HF_to_FLEXIO);

	FLEXIO_I2C_Type i2c_dev = {
		.flexioBase = FLEXIO0,
		.SDAPinIndex = FLEXIO_I2C_SDA_PIN,
		.SCLPinIndex = FLEXIO_I2C_SCL_PIN,
		.shifterIndex = {0, 1},	 /* Two 32-bit shift registers are used to transmit and receive data to/from the bus.
									Shifter 0 is used to transmit data and shifter 1 to receive data. */
		.timerIndex = {0, 1, 2}, /* Flexible 16-bit timers that provide the clocking, timing, and synchronization
									necessary for the protocol. A timer is used to generate the clock signal(SCL),
									a timer is used for the shift registers and a timer for timing like generating
									start/stop conditions. */
	};

	flexio_i2c_master_config_t config;
	FLEXIO_I2C_MasterGetDefaultConfig(&config);
	config.baudRate_Bps = FLEXIO_I2C_BAUDRATE;
	FLEXIO_I2C_MasterInit(&i2c_dev, &config, CLOCK_GetFlexioClkFreq());

	uint8_t buffer[MSGLEN + 1] = {0};
	flexio_i2c_master_transfer_t xfer = {0};
	xfer.slaveAddress = SLAVE_ADDRESS;
	xfer.dataSize = MSGLEN;
	xfer.data = buffer;

	flexio_i2c_master_handle_t handle;
	FLEXIO_I2C_MasterTransferCreateHandle(&i2c_dev, &handle, master_callback, NULL);

	while (1)
	{
		SDK_DelayAtLeastUs(500000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		for (int i = 0; i < MSGLEN; i++)
		{
			buffer[i] = 'a' + (rand() % 26);
		}

		complete = false;
		xfer.direction = kFLEXIO_I2C_Write;
		assert(kStatus_Success == FLEXIO_I2C_MasterTransferNonBlocking(&i2c_dev, &handle, &xfer));

		while (!complete)
		{
			;
		}

		printf("    Sent: %s\r\n", buffer);

		complete = false;
		xfer.direction = kFLEXIO_I2C_Read;
		assert(kStatus_Success == FLEXIO_I2C_MasterTransferNonBlocking(&i2c_dev, &handle, &xfer));

		while (!complete)
		{
			;
		}

		printf("Received: %s\r\n\r\n", buffer);
	}

	return 0;
}
