/**
 * @file example.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of array:
 *        1. Write a program to define a 4-element array of type int
 *        2. Initialize all elements with 0
 *        3. Read 4 integers from a terminal to fill the array
 *        4. Print the array to the terminal
 *
 *        Example:
 *           Enter 4 integers: 34, 5, 6, 7
 *           The array: {34, 5, 6, 7}
 *
 * @version 0.1
 * @date 2022-10-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

#define ARRAY_LEN 4

int main(void)
{
    int array[ARRAY_LEN] = {0};

    (void)printf("Enter 4 integers: ");
    (void)scanf("%d, %d, %d, %d", &array[0], &array[1], &array[2], &array[3]);

    (void)printf("The array: {");
    for (int i = 0; i < ARRAY_LEN - 1; i++)
    {
        (void)printf("%d, ", array[i]);
    }
    (void)printf("%d}\n", array[3]);

    return 0;
}