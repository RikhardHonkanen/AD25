#include <string.h>
#include "utility.h"

static utility_console_t console;

void utility_init(const utility_console_t *ptr)
{
    console = *ptr;
}
bool utility_string_read(char *str, size_t size)
{
    bool status = false;

    if ((str != NULL) && (size > 0))
    {
        char chr = 0;
        size_t i = 0;
        status = true;

        while (i < size)
        {
            chr = console.read();

            if (chr == '\n')
            {
                break;
            }
            else if (chr == console.eof)
            {
                status = false;
                break;
            }
            else
            {
                str[i++] = chr;
            }
        }

        str[i] = 0;
    }

    return status;
}

void utility_string_reverse(char *str)
{
    if (str != NULL)
    {
        size_t length = strlen(str);

        if (length > 0)
        {
            char temp;
            size_t left = 0;
            size_t right = length - 1;

            while (left < right)
            {
                temp = str[left];
                str[left] = str[right];
                str[right] = temp;
                right--;
                left++;
            }
        }
    }
}

bool utility_string_write(const char *str)
{
    bool status = false;

    if (str != NULL)
    {
        status = true;

        for (char *ptr = (char *)str; *ptr != '\0'; ptr++)
        {
            if (console.eof == console.write(*ptr))
            {
                status = false;
                break;
            }
        }
    }

    return status;
}