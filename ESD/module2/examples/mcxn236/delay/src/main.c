/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of timing using unistd a delay
 *        Look at: https://docs.zephyrproject.org/latest/kernel/services/scheduling/index.html
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <zephyr/kernel.h>

#define DELAY 1000

int main(void)
{
	while (1)
	{
		// k_sleep, k_msleep, and k_usleep suspend the thread
		// But k_busy_wait keeps the CPU busy, which can be useful for very short delays but it is not power-efficient for longer delays.

		k_msleep(DELAY); // Equivalent to: k_sleep(K_MSEC(DELAY)); and k_usleep(1000 * DELAY);

		printf("Hello World!\n");
	}

	return 0;
}
