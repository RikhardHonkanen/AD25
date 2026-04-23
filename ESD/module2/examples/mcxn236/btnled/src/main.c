/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of turning a LED connected to J2:11(P0_25) on using a push button connected to J2:17(P0_24).
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
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>

int main(void)
{
	const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(myled), gpios);
	const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_NODELABEL(mybtn), gpios);

	__ASSERT(gpio_is_ready_dt(&led), "LED) GPIO port is not ready!");
	__ASSERT(gpio_is_ready_dt(&button), "BTN) GPIO port is not ready!");

	__ASSERT(0 == gpio_pin_configure_dt(&button, GPIO_INPUT), "BTN) Failed to configure the gpio pin");
	__ASSERT(0 == gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE), "LED) Failed to configure the gpio pin");

	while (1)
	{
		__ASSERT_NO_MSG(0 == gpio_pin_set(led.port, led.pin, gpio_pin_get_dt(&button))); // gpio_pin_set_dt(&led, gpio_pin_get_dt(&button));
	}

	return EXIT_FAILURE;
}
