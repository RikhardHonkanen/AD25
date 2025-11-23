#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
/**
 * @brief
 * reads a lowercase letter and converts it to uppercase
 * prints two random numbers to the terminal
 *
 * @return int
 */
int main(void)
{
    char letter;
    printf("Enter a lowercase letter: ");
    scanf(" %c", &letter);
    char upper = toupper(letter);
    printf("Uppercase: %c\n", upper);

    // int num1 = rand();
    // int num2 = rand();
    srand(time(NULL));

    for (int i = 0; i < 2; i++)
    {
        int num = rand();
        printf("Random number: %d ", num);
    };

    // printf("Random numbers: %d %d\n", num1, num2);

    return 0;
}