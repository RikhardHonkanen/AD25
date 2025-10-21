/**
 * @file 17.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Write a program to make a 2 dimensional(3x3) array of type int.
 * Then ask the user to enter all the elements and then print the array to the output.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

void flush_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // flush bad input
}

int main(void)
{
    int usr_input;
    int matrix[3][3] = {{0}, {0}};

    // Populate matrix
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            (void)printf("\nEnter a number: ");
            while (1 != scanf(" %d", &usr_input)) // Only accept numbers
            {
                (void)printf("\nNot a number, try again.");
                flush_buffer();
            }
            matrix[i][j] = usr_input;
        }
    }

    // Print matrix
    for (size_t i = 0; i < 3; i++)
    {
        (void)printf("\n");

        for (size_t j = 0; j < 3; j++)
        {
            (void)printf("%d ", matrix[i][j]);
        }
    }

    return 0;
};