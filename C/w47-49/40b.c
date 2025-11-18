/**
 * @file 40.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Study the behaviour of the strtok function in string.h; http://www.cplusplus.com/reference/cstring/strtok/
 * Make a stand-alone and self-contained function like strtok to split a string using a delimiter which is one character.
 * @version 0.1
 * @date 2025-11-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static char *split_string(const char *str, const char *delims);
static bool char_in_delimiters(const char c, const char *delims);

static char *split_string(const char *str, const char *delims)
{
    static const char *p = NULL;

    if (str != NULL)
    {
        p = str;
    }

    if (p == NULL || *p == '\0')
    {
        return NULL;
    }

    const char *start = p;

    while (*p && !char_in_delimiters(*p, delims))
    {
        p++;
    }

    size_t len = p - start;
    char *token = malloc(len + 1);
    memcpy(token, start, len);
    token[len] = '\0';

    if (char_in_delimiters(*p, delims))
    {
        p++;
    }

    return token;
}

static bool char_in_delimiters(const char c, const char *delims)
{
    bool status = false;
    for (size_t i = 0; i < strlen(delims); i++)
    {
        if (c == delims[i])
        {
            status = true;
        }
    }
    return status;
}

int main(void)
{
    char *token;
    char *tokens[32];
    int count = 0;

    char *str = "- This, a sample string.";
    while ((token = split_string(str, " ,.-")) != NULL)
    {
        tokens[count++] = token;
        str = NULL;
    }

    (void)printf("Tokens:\n");
    for (size_t i = 0; i < count; i++)
    {
        if (strlen(tokens[i]) != 0)
        {
            (void)printf("%s\n", tokens[i]);
        }
        (void)free(tokens[i]);
    }

    return 0;
};