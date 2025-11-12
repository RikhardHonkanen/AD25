#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#define RANGE_MIN 0
#define RANGE_MAX 99
#define MAX_ATTEMPTS 10

int main(void)
{
	char new_game;

	srand(time(NULL));

	do
	{
		uint32_t attempts = 1;

		uint32_t secret = rand() % (RANGE_MAX + 1);

		(void)printf("\n******************************************\n");
		(void)printf("************** Game Started **************\n");
		(void)printf("******************************************\n");

		while (attempts <= MAX_ATTEMPTS)
		{
			uint32_t guess;

			(void)printf("\n%d) Guess a number [0, %d]: ", attempts, RANGE_MAX);

			if (1 == scanf("%d", &guess))
			{
				if (guess > 99)
				{
					(void)printf("Invalid number!");
				}
				else
				{
					if (guess == secret)
					{
						(void)printf("Well done! You guessed the secret.\n");
						break;
					}
					else if (attempts == MAX_ATTEMPTS)
					{
						(void)printf("Sorry, no more attempts. The secret number is %d.\n", secret);
					}
					else if (guess < secret)
					{
						(void)printf("Sorry, too small. Try again!\n");
					}
					else
					{
						(void)printf("Sorry, too big. Try again!\n");
					}

					attempts++;
				}
			}
			else
			{
				(void)printf("Invalid input!\n");

				char chr = 0;
				while ((chr != '\n') && (chr != EOF))
				{
					chr = getchar();
				}
			}
		}

		do
		{
			(void)printf("\nDo you want to play a new game? (y/n): ");

			(void)scanf(" %c", &new_game);
			new_game = toupper(new_game);

		} while ((new_game != 'Y') && (new_game != 'N'));

	} while (new_game == 'Y');

	return 0;
}
