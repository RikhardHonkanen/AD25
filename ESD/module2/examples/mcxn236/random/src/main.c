/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of using an ADC channel(LPADC0_A3) to seed the standard random number generator.
 *
 * @version 0.1
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "random.h"
#include <zephyr/kernel.h>

int main(void)
{
	if (!random_init())
	{
		printf("Falied to initialize the random number generator\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printf("Random = %010d\n", random_get());

		k_sleep(K_MSEC(500));
	}

	return EXIT_SUCCESS;
}
