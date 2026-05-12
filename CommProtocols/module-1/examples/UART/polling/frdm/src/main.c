/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART using polling.
 *        Look at: https://docs.zephyrproject.org/latest/hardware/peripherals/uart.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__uart__interface.html
 *
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/uart.h>

#define MSG_SIZE 8
#define UART_NODE DT_ALIAS(uart)

int main(void)
{
	const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);

	if (!device_is_ready(uart_dev))
	{
		printk("UART device not found!");

		exit(EXIT_FAILURE);
	}

	srand((unsigned)k_uptime_ticks());

	int i;
	char chr;

	while (1)
	{
		i = 0;
		printk("    Sent: ");
		while (i < MSG_SIZE)
		{
			chr = 'a' + (rand() % 26);
			uart_poll_out(uart_dev, chr);
			printk("%c", chr);
			i++;
		}

		i = 0;
		printk("\nReceived: ");
		while (i < MSG_SIZE)
		{
			if (0 == uart_poll_in(uart_dev, &chr))
			{
				printk("%c", chr);
				i++;
			}
		}

		printk("\n\n");
		k_msleep(1000);
	}

	return 0;
}
