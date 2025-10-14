/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Read an unsigned interger number greater than 1 from terminal using scanf.
 *        If the number is a multiply of 3 print Fizz to the terminal.
 *        If the number is a multiply of 5 print Buzz to the terminal.
 *        If the number is a multiply of 3 x 5 print FizzBuzz to the terminal.
 *        Otherwise print the number to the terminal.
 *
 *        - No magic numbers & strings!
 *        - If scanf cannot read an integer print error message to the terminal
 *        - If the entered number is 0 print an error message to the terminal
 *
 * @version 0.1
 * @date 2022-10-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>

#define FIZZ_NUM 3
#define FIZZ_TXT "Fizz"

#define BUZZ_NUM 5
#define BUZZ_TXT "Buzz"

#define FIZZBUZZ_NUM (FIZZ_NUM * BUZZ_NUM)
#define FIZZBUZZ_TXT "FizzBuzz"

int main(void)
{
    unsigned int number;

    (void)printf("Enter a positive number greater than 0: ");

    if (1 == scanf("%d", &number))
    {
        if (number == 0)
        {
            (void)printf("The number shall be greater than 0!\n");
        }
        else if (number % FIZZBUZZ_NUM == 0)
        {
            (void)printf(FIZZBUZZ_TXT);
        }
        else if (number % BUZZ_NUM == 0)
        {
            (void)printf(BUZZ_TXT);
        }
        else if (number % FIZZ_NUM == 0)
        {
            (void)printf(FIZZ_TXT);
        }
        else
        {
            (void)printf("%d", number);
        }
    }
    else
    {
        (void)printf("Failed to read a number!\n");
    }
    return 0;
}