/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of making a LED connected to J2:17(P0_24) blinking every 0.5s using a timer.
 *        Look at: https://docs.zephyrproject.org/latest/build/dts/zephyr-user-node.html
 *                 https://docs.zephyrproject.org/latest/doxygen/html/group__gpio__interface.html
 *                 https://docs.zephyrproject.org/latest/samples/basic/blinky/README.html
 *                 https://docs.zephyrproject.org/latest/kernel/services/timing/timers.html
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
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>

#define TIMER_PERIOD_MS 500 // Timer interval in milliseconds
#define TIME_INITIAL_DURATION 3000
#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, led_gpios);

void on_expire(struct k_timer *)
{
	__ASSERT(0 == gpio_pin_toggle_dt(&led), "Falied to toggle the LED");
}

int main(void)
{
	__ASSERT(gpio_is_ready_dt(&led), "GPIO port is not ready!");

	__ASSERT(0 == gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE), "Failed to configure the gpio pin");

	struct k_timer timer;

	// Initialize the timer
	k_timer_init(&timer, on_expire, NULL);

	// Start the timer to run periodically
	k_timer_start(&timer, K_MSEC(TIME_INITIAL_DURATION), K_MSEC(TIMER_PERIOD_MS));

	while (1)
	{
		printk("Hello World!\n");
		k_busy_wait(1000000); // keeps the CPU busy
	}

	return 0;
}
