#include <math.h>
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#define NUM_MIN 2
#define NUM_MAX 1000
#define FIRST_PRIME 2
#define STRINGIFY(x) #x
#define LENGTH(x) sizeof(STRINGIFY(x))

static int get_number(void)
{
	size_t i = 0;
	char chr = 0;
	int number = 0;

	while (i < LENGTH(NUM_MAX))
	{
		chr = getchar();

		if ((chr >= '0') && (chr <= '9'))
		{
			i++;
			putchar(chr);
			number = 10 * number + (chr - '0');
		}
		else if (chr == '\r' || chr == '\n')
		{
			break;
		}
	};

	printf("\r\n");

	return number;
}

static bool isprime(int number)
{
	bool prime = true;

	if (number < FIRST_PRIME)
	{
		prime = false;
	}
	else
	{
		if (number > FIRST_PRIME)
		{
			int counter = FIRST_PRIME;
			int max_number = ceil(sqrt(number));

			while (counter <= max_number)
			{
				if (number % counter == 0)
				{
					prime = false;
					break;
				}

				counter++;
			}
		}
	}

	return prime;
}

int main(void)
{
	// Init board hardware
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	printf("\r\n");

	while (1)
	{
		printf("Enter a number in the range of [%d, %d]: ", NUM_MIN, NUM_MAX);

		int number = get_number();

		if ((number >= NUM_MIN) && (number <= NUM_MAX))
		{
			printf("%d is %sprime!\r\n\r\n", number, isprime(number) ? "" : "not ");
		}
	}

	return 0;
}
