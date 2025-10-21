/**
 * @file 22.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void)
{
    const char *s1 = "Roter";
    char s2[6] = "Roger";
    char *p = (char *)s1;
    char **pp = &p;

    *pp = s2;
    *p = 'P';
    **pp = 'J';
    *(*pp + 2) = 'k';

    printf("%s\n", p);

    uint8_t array[4] = {1, 2, 3, 4};
    bool a = ((void *)array == (void *)&array);
    bool b = ((void *)(array + 1) == (void *)(&array + 1));

    printf("%s ", a ? "Hello" : "Bye");
    printf("%s!\n", b ? "Stefan" : "Linda");

    return 0;
};