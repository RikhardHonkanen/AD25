/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of UART using interrupts.
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
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#define MSG_SIZE 8
#define UART_NODE DT_ALIAS(uart)

// Queue to store up to 4 messages (aligned to 1-byte boundary)
K_MSGQ_DEFINE(msgq, MSG_SIZE, 4, 1);

static void on_receive(const struct device *udev, void *)
{
	if ((1 == uart_irq_update(udev)) && (1 == uart_irq_rx_ready(udev)))
	{
		uint8_t chr;
		static int pos = 0;
		static uint8_t buffer[MSG_SIZE];

		// Read until the FIFO gets empty
		while (sizeof(chr) == uart_fifo_read(udev, &chr, sizeof(chr)))
		{
			buffer[pos++] = chr;

			if (pos == MSG_SIZE)
			{
				pos = 0;

				(void)k_msgq_put(&msgq, buffer, K_FOREVER);
			}
		}
	}
}

int main(void)
{
	const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);

	if (!device_is_ready(uart_dev))
	{
		printk("UART device not found!");

		exit(EXIT_FAILURE);
	}

	// Configure interrupt and callback to receive data
	if (0 != uart_irq_callback_user_data_set(uart_dev, on_receive, NULL))
	{
		printk("Failed to configure interrupt and callback to receive data \n");

		exit(EXIT_FAILURE);
	}

	uart_irq_rx_enable(uart_dev);

	srand((unsigned)k_uptime_ticks());

	uint8_t chr;
	uint8_t buffer[MSG_SIZE];

	while (1)
	{
		printk("\n    Sent: ");
		for (int i = 0; i < MSG_SIZE; i++)
		{
			chr = 'a' + (rand() % 26);
			uart_poll_out(uart_dev, chr);
			printk("%c", chr);
		}

		printk("\nReceived: ");
		if (0 == k_msgq_get(&msgq, buffer, K_FOREVER))
		{
			for (int i = 0; i < MSG_SIZE; i++)
			{
				printk("%c", buffer[i]);
			}
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
