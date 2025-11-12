/**
 * @file exercise-16.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to get a string from the standard input, then
 *        print the string to the output in reverse order.
 *
 *        Examaple:
 *            Enter a string: ABCDEF
 *            Reversed string: FEDCBA
 *
 * @version 0.1
 * @date 2021-02-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <string.h>

#define LENGTH 8

int main(void)
{
    char string[LENGTH + 1] = {0};

    // Read the string from the standard input
    (void)printf("Enter a string: ");
    (void)fgets(string, sizeof(string), stdin); // fgets may capture a newline
    int len = strlen(string);
    if (string[len - 1] == '\n')
    {
        len--;
        string[len] = '\0';
    }

    // Reverse the string
    int left = 0, right = len - 1;
    while (left < right)
    {
        char temp = string[left];
        string[left] = string[right];
        string[right] = temp;

        right--;
        left++;
    }

    // Print the reversed string to the output
    (void)printf("Reversed string: %s\n", string);

    return 0;
}
