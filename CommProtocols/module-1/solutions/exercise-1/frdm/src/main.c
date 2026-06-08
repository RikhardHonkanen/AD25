#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/console/console.h>

#define RTC_NODE DT_ALIAS(rtc)
#define UART_NODE DT_ALIAS(uart)

static int read_command(void)
{
	int chr = EOF;

	while ((chr != 'S') && (chr != 'D') && (chr != '\n'))
	{
		chr = toupper(console_getchar());

		if (chr == '\r')
		{
			chr = '\n';
		}
	}

	console_putchar(chr);

	return chr;
}

int main(void)
{
	const struct device *rtc_dev = DEVICE_DT_GET(RTC_NODE);

	if (!device_is_ready(rtc_dev))
	{
		printk("The RTC device is not ready.\r\n");
		exit(EXIT_FAILURE);
	}

	const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);

	if (!device_is_ready(uart_dev))
	{
		printk("UART device not found.\r\n");
		exit(EXIT_FAILURE);
	}

	if (0 != console_init())
	{
		printk("Failed to initialize console.\r\n");
		exit(EXIT_FAILURE);
	}

	char command;
	struct rtc_time datetime = {0};

	while (1)
	{
		printf("S) Synchronize datetime\r\n"
			   "D) Display datetime\r\n"
			   "Enter a command: ");

		command = read_command();

		switch (command)
		{
		case 'S':
		{
			uart_poll_out(uart_dev, command);

			uint8_t *ptr = (uint8_t *)&datetime;

			while ((ptr - (uint8_t *)&datetime) < sizeof(struct tm))
			{
				if (0 == uart_poll_in(uart_dev, ptr))
				{
					ptr++;
				}
			}

			if (datetime.tm_year > 0)
			{
				datetime.tm_nsec = 0;

				if (0 != rtc_set_time(rtc_dev, &datetime))
				{
					printk("Failed to set the RTC time!\r\n");
					exit(EXIT_FAILURE);
				}

				printk("\r\nSynchronized!\r\n\r\n");
			}
			else
			{
				printk("\r\nSynchronization failed!\r\n\r\n");
			}
		}
		break;

		case 'D':
		{
			if (0 != rtc_get_time(rtc_dev, &datetime))
			{
				printk("\r\nFailed to get the RTC time!\r\n");
				exit(EXIT_FAILURE);
			}
			else
			{
				printk("\r\n%04d-%02d-%02d %02d:%02d:%02d\r\n\r\n",
					   datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday,
					   datetime.tm_hour, datetime.tm_min, datetime.tm_sec);
			}
		}
		break;

		default:
			printf("\r\n");
			break;
		}
	}

	return EXIT_SUCCESS;
}
