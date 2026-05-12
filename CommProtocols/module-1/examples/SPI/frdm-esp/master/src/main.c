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
#include <ctype.h>
#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/spi.h>

#define MSG_SIZE 8
#define SPI_NODE DT_PROP(DT_PATH(zephyr_user), spi)
#define SPI_CLOCK_RATE DT_PROP(DT_PATH(zephyr_user), speed)

int main(void)
{
	const struct device *spi_dev = DEVICE_DT_GET(SPI_NODE);

	if (!device_is_ready(spi_dev))
	{
		printk("SPI device not ready!");
		exit(EXIT_FAILURE);
	}

	const struct spi_config config = {.frequency = SPI_CLOCK_RATE, .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8)};

	uint8_t tx_buffer[MSG_SIZE + 1] = {0};
	struct spi_buf tx_buf = {.buf = tx_buffer, .len = MSG_SIZE};
	struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};

	uint8_t rx_buffer[MSG_SIZE + 1] = {0};
	struct spi_buf rx_buf = {.buf = rx_buffer, .len = MSG_SIZE};
	struct spi_buf_set rx = {.buffers = &rx_buf, .count = 1};

	srand((uint32_t)k_uptime_ticks());

	while (1)
	{
		for (int i = 0; i < MSG_SIZE; i++)
		{
			tx_buffer[i] = 'a' + (rand() % 26);
		}

		printk("    Sent: ");
		if (0 == spi_write(spi_dev, &config, &tx))
		{
			printk("%s\nReceived: ", tx_buffer);

			k_msleep(1); // Wait on the slave

			if (0 == spi_read(spi_dev, &config, &rx))
			{
				printk("%s", rx_buffer);
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
