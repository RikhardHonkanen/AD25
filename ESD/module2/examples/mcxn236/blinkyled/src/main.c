/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of making the red led blinking every 0.5s using delays.
 *        Look at: https://docs.zephyrproject.org/latest/build/dts/index.html
 *                 https://docs.zephyrproject.org/latest/build/dts/api/bindings/led/gpio-leds.html
 *                 https://docs.zephyrproject.org/latest/hardware/peripherals/gpio.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__gpio__interface.html
 *                 https://docs.zephyrproject.org/latest/samples/basic/blinky/README.html
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

// led0 === red_led
// led1 === green_led
// led2 === blue_led
#define RED_LED_NODE DT_ALIAS(led0) // The devicetree node identifier for the "led0" alias.

int main(void)
{
	const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(RED_LED_NODE, gpios);

	__ASSERT(gpio_is_ready_dt(&led), "GPIO port is not ready!");

	__ASSERT(0 == gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE), "Failed to configure the gpio pin");

	while (1)
	{
		__ASSERT(0 == gpio_pin_toggle_dt(&led), "Falied to toggle the LED");

		k_msleep(500); // 0.5s delay
	}

	return 0;
}
