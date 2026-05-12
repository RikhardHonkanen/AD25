/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of I2C in master mode.
 *        Look at: https://docs.zephyrproject.org/latest/hardware/peripherals/i2c.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__i2c__interface.html
 *
 * @version 0.1
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/i2c.h>

#define MSG_SIZE 8
#define I2C_NODE DT_PROP(DT_PATH(zephyr_user), i2c)
#define SLAVE_ADDRESS DT_PROP(DT_PATH(zephyr_user), slave_address)

int main(void)
{
	const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

	if (!device_is_ready(i2c_dev))
	{
		printk("I2C device not found!");
		exit(EXIT_FAILURE);
	}

	if (0 != i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST) | I2C_MODE_CONTROLLER))
	{
		printk("I2C device config failed\n");
		exit(EXIT_FAILURE);
	}

	k_msleep(2000);

	printk("\n============= I2C Devices =============\n");
	for (int addr = 0; addr < 128; addr++)
	{
		uint8_t value = 0;
		if (0 == i2c_write(i2c_dev, &value, 0, addr))
		{
			printk("I2C device found at address 0x%02X\n", addr);
		}
	}
	printk("=======================================\n\n");

	srand((unsigned)k_uptime_ticks());

	uint8_t buffer[MSG_SIZE + 1] = {0};

	while (1)
	{
		for (int i = 0; i < MSG_SIZE; i++)
		{
			buffer[i] = 'a' + (rand() % 26);
		}

		printk("    Sent: ");
		if (0 == i2c_write(i2c_dev, buffer, MSG_SIZE, SLAVE_ADDRESS))
		{
			printk("%s\nReceived: ", buffer);
			if (0 == i2c_read(i2c_dev, buffer, MSG_SIZE, SLAVE_ADDRESS))
			{
				printk("%s", buffer);
			}
			else
			{
				printk("Failed");
			}
			printk("\n");
		}
		else
		{
			printk("Failed");
		}
		printk("\n");

		k_msleep(1000);
	}

	return 0;
}
