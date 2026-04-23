/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A C++ example of timing using the system ticks
 *        Look at: https://docs.zephyrproject.org/latest/kernel/services/timing/clocks.html
 *
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/sys/printk.h>

int main(void)
{
	uint64_t counter{0};
	uint64_t current, prevoius{0};
	constexpr int64_t INTERVAL{1000000}; // 1 second

	while (1)
	{
		current = k_ticks_to_us_floor64(k_uptime_ticks());

		if (current - prevoius >= INTERVAL)
		{
			printk("Counter value at %llu us = %llu\n", current, counter);
			prevoius = current;
		}

		counter++;
	}

	return 0;
}
