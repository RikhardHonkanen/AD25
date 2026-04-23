#include "stdio.h"
#include <ctype.h>
#include <stddef.h>
#include "utility.h"

bool utility_read_int(int *number)
{
    bool status = false;

    if (number != NULL)
    {
        char chr;

        do
        {
            chr = getchar();
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
                chr = getchar();

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