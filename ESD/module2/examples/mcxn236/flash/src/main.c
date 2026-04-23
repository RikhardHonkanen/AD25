/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of flash erasing, writing and reading
 *        Look at: https://docs.zephyrproject.org/latest/doxygen/html/group__flash__interface.html
 *                 https://docs.zephyrproject.org/latest/hardware/peripherals/flash.html
 *
 * @version 0.1
 * @date 2025-05-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/console/console.h>

#define NAME_MAX_LEN 31

#define FLASH_SIZE (1 << 20)
#define FLASH_ERASE_BLOCK_SIZE (1 << 13)
#define FLASH_REGION_OFFSET (FLASH_SIZE - FLASH_ERASE_BLOCK_SIZE)

static char read_command(void)
{
	int command = 0;
	while ((command != 'S') && (command != 'P') && (command != 'R') && (command != '\n'))
	{
		command = toupper(console_getchar());

		if (command == '\r')
		{
			command = '\n';
		}
	}

	printf("%c", command);

	return command;
}

static void read_string(char *str, int max_len)
{
	int i = 0;
	int chr = 0;

	while (i < max_len)
	{
		chr = console_getchar();

		console_putchar(chr);

		if ((chr == '\n') || (chr == '\r'))
		{
			break;
		}
		else
		{
			str[i] = chr;
			i++;
		}
	}
}

int main(void)
{
	const struct device *flash_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));

	if (!device_is_ready(flash_dev))
	{
		printf("%s: device not ready.\n", flash_dev->name);
		exit(EXIT_FAILURE);
	}

	console_init();

	while (1)
	{
		printf("S) Set Name\n"
			   "P) Print Name\n"
			   "R) Reset the MCU\n"
			   "Enter A Command: ");

		switch (read_command())
		{
		case 'S':
		{
			printf("\r\nEnter Name: ");

			char name[NAME_MAX_LEN + 1] = {0};

			read_string(name, NAME_MAX_LEN);

			if (0 == flash_erase(flash_dev, FLASH_REGION_OFFSET, FLASH_ERASE_BLOCK_SIZE))
			{
				if (0 == flash_write(flash_dev, FLASH_REGION_OFFSET, name, sizeof(name)))
				{
					printf("\r\nName %s\r\n\r\n", (name[0] == 0) ? "Deleted" : "Stored");
				}
				else
				{
					printf("\r\nFlash write failed!\r\n\r\n");
				}
			}
			else
			{
				printf("\r\nFlash erase failed!\r\n\r\n");
			}

			break;
		}

		case 'P':
		{
			char name[NAME_MAX_LEN + 1] = {0};

			if (0 == flash_read(flash_dev, FLASH_REGION_OFFSET, name, sizeof(name)))
			{
				printf("\r\nName: %s\r\n\r\n", name);
			}
			else
			{
				printf("\r\nFlash read failed!\r\n\r\n");
			}

			break;
		}

		case 'R':
			sys_reboot(SYS_REBOOT_COLD);
			break;

		default:
			printf("\r\n");
			break;
		}
	}

	return EXIT_SUCCESS;
}
