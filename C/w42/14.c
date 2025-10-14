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
#include <stdint.h>

#define MAX_SECRET 99
#define GUESSES 10

void flush(void)
{
    int c;
    while (c = getchar() != '\n' && c != EOF)
    {
    } // flush bad input
}

int main(void)
{
    srand(time(NULL));

start:
    printf("\n******************************************\n************** Game Started **************\n******************************************\n");
    uint32_t guess;
    int remaining_guesses = GUESSES;
    // int secret = rand() % (MAX_SECRET + 1);
    int secret = 74;

    do
    {
        (void)printf("\nAttempt %d, guess a number between 0 and %d: ", (GUESSES + 1) - remaining_guesses, MAX_SECRET);
        if (!(1 == scanf(" %d", &guess))) // Only accept numbers
        {
            printf("\nNot a number, try again.");
            flush();
            continue;
        }
        if (guess > 99)
        {
            printf("\nOut of range (0-99), try again.");
            continue;
        }

        if (guess == secret)
        {
            break;
        }
        if (guess < secret)
        {
            (void)printf("\nToo low! ");
        }
        else if (guess > secret)
        {
            (void)printf("\nToo high! ");
        }

        remaining_guesses--;
        if (remaining_guesses < 1)
        {
            (void)printf("\nOut of guesses :( "); // Game lost
            goto end;
        }
    } while (guess != secret);

    (void)printf("\nCorrect!"); // Game won

end:
    char again;
    (void)printf("\n\nPlay again (y/n)? ");
    (void)scanf(" %c", &again);
    if (again == 'y' || again == 'Y')
    {
        goto start;
    }

    return 0;
}