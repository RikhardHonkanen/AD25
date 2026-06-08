/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of SPI in master mode.
 *        Look at: https://docs.zephyrproject.org/latest/doxygen/html/group__spi__interface.html
 *
 * @version 0.1
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/console/console.h>

#define RTC_NODE DT_ALIAS(rtc)
#define SPI_NODE DT_PROP(DT_PATH(zephyr_user), spi)
#define SPI_CLOCK_RATE DT_PROP(DT_PATH(zephyr_user), speed)

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
	if (0 != console_init())
	{
		printk("Failed to initialize console.\n");
		exit(EXIT_FAILURE);
	}

	const struct device *rtc_dev = DEVICE_DT_GET(RTC_NODE);
	if (!device_is_ready(rtc_dev))
	{
		printk("The RTC device is not ready.\n");
		exit(EXIT_FAILURE);
	}

	const struct device *spi_dev = DEVICE_DT_GET(SPI_NODE);

	if (!device_is_ready(spi_dev))
	{
		printk("SPI device not ready!");
		exit(EXIT_FAILURE);
	}

	const struct spi_config config = {.frequency = SPI_CLOCK_RATE, .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8)};

	char command;
	struct rtc_time datetime = {0};

	struct spi_buf tx_buf = {.buf = &command, .len = sizeof(command)};
	struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};

	struct spi_buf rx_buf = {.buf = &datetime, .len = sizeof(struct tm)};
	struct spi_buf_set rx = {.buffers = &rx_buf, .count = 1};

	while (1)
	{
		printf("S) Synchronize datetime\n"
			   "D) Display datetime\n"
			   "Enter a command: ");

		command = read_command();

		switch (command)
		{
		case 'S':
		{
			if (0 == spi_write(spi_dev, &config, &tx))
			{
				k_msleep(1); // Wait on the slave

				if (0 == spi_read(spi_dev, &config, &rx))
				{
					if (datetime.tm_year > 0)
					{
						datetime.tm_nsec = 0;

						if (0 != rtc_set_time(rtc_dev, &datetime))
						{
							printk("Failed to set the RTC time!\n");
							exit(EXIT_FAILURE);
						}

						printk("\nSynchronized!\n\n");
					}
					else
					{
						printk("\nSynchronization failed!\n\n");
					}
				}
				else
				{
					printf("\nFailed to read\n");
				}
			}
			else
			{
				printf("\nFailed to write\n");
			}
		}
		break;

		case 'D':
		{
			if (0 != rtc_get_time(rtc_dev, &datetime))
			{
				printk("\nFailed to get the RTC time!\n");
				exit(EXIT_FAILURE);
			}
			else
			{
				printk("\n%04d-%02d-%02d %02d:%02d:%02d\n\n",
					   datetime.tm_year + 1900, datetime.tm_mon + 1, datetime.tm_mday,
					   datetime.tm_hour, datetime.tm_min, datetime.tm_sec);
			}
		}
		break;

		default:
			printf("\n");
			break;
		}
	}

	return EXIT_SUCCESS;
}