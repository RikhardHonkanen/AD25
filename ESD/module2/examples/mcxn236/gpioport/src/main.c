/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of writing to multiple pins in a port simultaneously.
 *        Look at: https://docs.zephyrproject.org/latest/build/dts/zephyr-user-node.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__gpio__interface.html
 *                 https://docs.zephyrproject.org/latest/build/dts/howtos.html#set-devicetree-overlays
 *                 https://docs.zephyrproject.org/latest/build/dts/index.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)
#define PORT DT_PHANDLE(ZEPHYR_USER_NODE, port)
#define ACTIVE_MODE DT_PROP(ZEPHYR_USER_NODE, mode)

int main(void)
{
	gpio_port_pins_t pins = 0U;
	const struct device *port = DEVICE_DT_GET(PORT);

	if (!device_is_ready(port))
	{
		printk("The GPIO port is not ready!\r\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		const int temp[] = DT_PROP(ZEPHYR_USER_NODE, pins);
		for (int i = 0; i < sizeof(temp) / sizeof(temp[0]); i++)
		{
			pins |= BIT(temp[i]);

			if (0 != gpio_pin_configure(port, temp[i], ACTIVE_MODE | GPIO_OUTPUT_INACTIVE))
			{
				printk("Failed to configure the pins!\r\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	while (1)
	{
		if (0 != gpio_port_set_bits(port, pins))
		{
			printk("Failed to set the pins!\r\n");
			exit(EXIT_FAILURE);
		}

		k_msleep(1000);

		if (0 != gpio_port_clear_bits(port, pins))
		{
			printk("Failed to set the pins!\r\n");
			exit(EXIT_FAILURE);
		}

		k_msleep(1000);
	}

	return EXIT_SUCCESS;
}
