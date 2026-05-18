#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

int main(void)
{
    utility_console_t console = {.eof = EOF, .read = getchar, .write = putchar};

    utility_init(&console);

    if (!utility_string_write("Enter a string: "))
    {
        exit(EXIT_FAILURE);
    }

    char string[129] = {0};

    if (utility_string_read(string, sizeof(string) - 1))
    {
        utility_string_reverse(string);

        if (!utility_string_write("Reversed string: ") || !utility_string_write(string) || !utility_string_write("\n"))
        {
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (!utility_string_write("Failed to read from input!\n"))
        {
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;

    return 0;
}
