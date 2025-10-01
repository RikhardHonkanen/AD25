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

    srand(time(NULL));

    int num1 = rand();
    int num2 = rand();

    printf("Random numbers: %d %d\n", num1, num2);

    return 0;
}