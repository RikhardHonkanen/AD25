/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a LIN slave using UART.
 *        Look at: https://docs.zephyrproject.org/latest/hardware/peripherals/uart.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__uart__interface.html
 *
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#define MSGID 50

#define DLEN(x) ((x <= 31) ? 2 : ((x <= 47) ? 4 : 8))
#define PID(x) (x | ((((x >> 0) ^ (x >> 1) ^ (x >> 2) ^ (x >> 4)) & 1) << 6) | ((!(((x >> 1) ^ (x >> 3) ^ (x >> 4) ^ (x >> 5)) & 1)) << 7))

static void prepare_message(uint8_t pid, uint8_t *data, int size, uint8_t *checksum)
{
	uint16_t temp = pid;

	for (int i = 0; i < size; i++)
	{
		data[i] = 'A' + (rand() % 26);

		temp += data[i];
		if (temp > 255)
		{
			temp -= 255;
		}
	}

	*checksum = ~temp;
}

int main(void)
{
	const struct device *uart_dev = DEVICE_DT_GET(DT_ALIAS(uart));

	if (!device_is_ready(uart_dev))
	{
		printk("UART device not found\n");

		exit(EXIT_FAILURE);
	}

	printk("LIN Slave Started\n");

	uint8_t checksum;
	uint8_t chr, data[DLEN(MSGID)];

	prepare_message(PID(MSGID), data, sizeof(data), &checksum);

	while (1)
	{
		chr = 0;
		if ((0 == uart_poll_in(uart_dev, &chr)) && (chr == 0))
		{
			if (UART_ERROR_FRAMING & uart_err_check(uart_dev)) /* Detect BREAK (Framing Error) */
			{
				while (chr == 0)
				{
					(void)uart_poll_in(uart_dev, &chr);
				}

				if (chr == 0x55)
				{
					while (0 != uart_poll_in(uart_dev, &chr))
					{
						;
					}

					if (chr == PID(MSGID))
					{
						for (int i = 0; i < DLEN(MSGID); i++)
						{
							uart_poll_out(uart_dev, data[i]);
						}
						uart_poll_out(uart_dev, checksum);

						prepare_message(PID(MSGID), data, sizeof(data), &checksum);
					}
					else
					{
						printk("PID mismatch error\n");
					}
				}
				else
				{
					printk("Failed to synchronized\n");
				}
			}
		}
	}

	return 0;
}
