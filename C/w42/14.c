/**
 * @file 14.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Get a user guess a secret random number in the range of 0 and 99.
 * The user shall have maximum 10 attempts to guess the secret number.
 * After every guess a message should be presented informing the user if the entered value was correct, too small or too big.
 * After each game the program shall offer a new game
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
start:
    printf("******************************************\n************** Game Started **************\n******************************************");
    int guess;
    int remaining_guesses = 10;
    int max = 99;
    srand(time(NULL));
    int secret = rand() % (max + 1);

    do
    {
        (void)printf("\nGuess a number between 0 and 99: ");
        (void)scanf(" %d", &guess);
        // fflush(stdin);
        // int valid = 1; // assume valid until proven otherwise
        // for (int i = 0; i < strlen(guess); i++)
        // {
        //     if (!isdigit((unsigned char)guess[i]))
        //     {
        //         valid = 0;
        //         break; // break out of inner loop
        //     }
        // }

        // if (!valid)
        // {
        //     printf("Invalid input! Please enter only digits.\n");
        //     continue;
        // }

        if (guess < secret)
        {
            (void)printf("\nToo low!: ");
        }
        else if (guess > secret)
        {
            (void)printf("\nToo high!: ");
        }
        remaining_guesses--;
        if (remaining_guesses < 1)
        {
            (void)printf("\nOut of guesses :( ");
            goto end;
        }
    } while (guess != secret);
    (void)printf("\nCorrect!");

end:
    char again;
    (void)printf("\nPlay again (y/n)? ");
    (void)scanf(" %c", &again);
    if (again == 'y' || again == 'Y')
    {
        goto start;
    }

    return 0;
}