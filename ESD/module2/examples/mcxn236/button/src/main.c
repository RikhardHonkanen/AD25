/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a bouncing push button connected to J2:17(P0_24).
 *        Look at: https://docs.zephyrproject.org/latest/build/dts/zephyr-user-node.html#gpios
 *                 https://docs.zephyrproject.org/latest/samples/basic/button/README.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

int main(void)
{
	const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, button_gpios);

	if (!gpio_is_ready_dt(&button))
	{
		printf("GPIO port is not ready!\n");
		exit(EXIT_FAILURE);
	}

	if (0 != gpio_pin_configure_dt(&button, GPIO_INPUT))
	{
		printf("Failed to configure the gpio pin\n");
		exit(EXIT_FAILURE);
	}

	int current_state;
	uint32_t count = 0;
	uint32_t previous_count = 0;
	const int PRESSED = (GPIO_ACTIVE_LOW & button.dt_flags) ? 1 : 0;
	int previous_state = (GPIO_ACTIVE_LOW & button.dt_flags) ? 0 : 1;

	while (1)
	{
		current_state = gpio_pin_get_dt(&button);

		if (current_state != previous_state)
		{
			if (current_state == PRESSED)
			{
				count++;
			}
			previous_state = current_state;
		}
		else if (count != previous_count)
		{
			printf("count: %u\n", count);
			previous_count = count;
		}
		else
		{
			;
		}
	}

	return EXIT_FAILURE;
}
