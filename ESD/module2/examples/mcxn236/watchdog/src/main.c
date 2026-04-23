/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using the watchdog to reset the microcontroller.
 *        Look at: https://docs.zephyrproject.org/latest/doxygen/html/group__watchdog__interface.html
 *
 * @version 0.1
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/watchdog.h>

#define WDT_WINDOW_MAX 5000U

int main(void)
{
	const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

	if (!device_is_ready(wdt))
	{
		printk("%s: device not ready.", wdt->name);
		return EXIT_FAILURE;
	}

	struct wdt_timeout_cfg wdt_cfg = {
		/* Expire watchdog after max window */
		.window.max = WDT_WINDOW_MAX,
		.flags = WDT_FLAG_RESET_SOC,
	};

	int wdt_channel_id = wdt_install_timeout(wdt, &wdt_cfg);

	if (wdt_channel_id < 0)
	{
		printk("Watchdog install error.");
		return EXIT_FAILURE;
	}

	if (0 != wdt_setup(wdt, WDT_OPT_PAUSE_HALTED_BY_DBG))
	{
		printk("Watchdog setup error.");
		return EXIT_FAILURE;
	}

	/* Feeding watchdog. */

	printk("\n===== Feeding the watchdog =====\n");
	if (0 != wdt_feed(wdt, wdt_channel_id))
	{
		printk("Watchdog feeding error.");
		return EXIT_FAILURE;
	}

	// Simulate a task
	printk("Task started...\n");
	for (volatile int i = 0; i < 0x7FFFFF; i++)
	{
		__asm("nop");
	}
	printk("Task performed!\n");

	/* Waiting for the SoC reset. */
	printk("====== Waiting for reset =======\n\n");

	while (1)
	{
		k_sleep(K_FOREVER);
	}

	return EXIT_SUCCESS;
}
