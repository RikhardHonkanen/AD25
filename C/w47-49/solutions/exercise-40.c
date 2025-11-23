#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char *split_string(const char *str, const char delim);

int main(void)
{
    const char delimiter = ' ';
    const char *string = "This is a  test!";

    char *ptr = split_string(string, delimiter);
    while (ptr != NULL)
    {
        (void)printf("%s\n", ptr);
        ptr = split_string(NULL, delimiter);
    }

    return 0;
}

static char *split_string(const char *str, const char delim)
{
    static size_t length = 0;
    static char *result = NULL;
    static char *pstr = NULL;

    if (str != NULL)
    {
        pstr = (char *)str;
        if (result != NULL)
        {
            free(result);
            result = NULL;
        }
        length = strlen(str) + 1;
        result = (char *)malloc(length);
    }

    if (result != NULL)
    {
        (void)memset(result, 0, length);

        size_t i = 0;

        while ((*pstr != '\0') && (*pstr != delim))
        {
            result[i++] = *pstr;
            pstr++;
        }

        while (*pstr == delim)
        {
            pstr++;
        }

        if (0 == strlen(result))
        {
            pstr = NULL;
            free(result);
            result = NULL;
        }
    }

    return result;
}

