#include <stdio.h>
#include "utility.h"

int main(void)
{
    int number = 0;

    printf("Enter an integer: ");

    if (utility_read_int(&number))
    {
        printf("The entered number: %d\n", number);
    }
    else
    {
        printf("Invalid input\n");
    }

    return 0;
}