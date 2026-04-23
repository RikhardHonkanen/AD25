#include <ctype.h>
#include <stddef.h>
#include "utility.h"

static utility_getchar_t get_char;

void utility_init(utility_getchar_t fptr)
{
    get_char = fptr;
}

bool utility_read_int(int *number)
{
    bool status = false;

    if (number != NULL)
    {
        char chr;

        do
        {
            chr = get_char();
        } while (chr == ' ');

        if ((chr == '-') || (chr == '+') || isdigit(chr))
        {
            *number = 0;
            bool negative = false;

            if (isdigit(chr))
            {
                status = true;
                *number *= 10;
                *number += chr - '0';
            }
            else
            {
                negative = (chr == '-');
            }

            while (1)
            {
                chr = get_char();

                if (isdigit(chr))
                {
                    status = true;
                    *number *= 10;
                    *number += chr - '0';
                }
                else
                {
                    if (chr != '\n')
                    {
                        status = status && (chr == ' ');
                    }

                    break;
                }
            }

            if (negative)
            {
                *number *= -1;
            }
        }
    }

    return status;
}