#include <string.h>
#include "utility.h"

#define STRLEN 16U

static utility_console_t output = {0};

void utility_init(const utility_console_t *ptr)
{
    output = *ptr;
}

bool utility_write_int(int number)
{
    char string[16] = {0};
    int i = sizeof(string) - 2;
    bool negative = (number < 0);

    if (negative)
    {
        number *= -1;
    }

    if (number == 0)
    {
        string[i] = '0';
        i--;
    }

    while (number != 0)
    {
        string[i] = (number % 10) + '0';
        number /= 10;
        i--;
    }

    if (negative)
    {
        string[i] = '-';
        i--;
    }

    bool status = true;

    for (char *ptr = (char *)&string[i + 1]; *ptr != '\0'; ptr++)
    {
        if (output.eof == output.write(*ptr))
        {
            status = false;
            break;
        }
    }

    return status;
}