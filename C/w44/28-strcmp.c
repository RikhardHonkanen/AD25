/**
 * @file 28.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a function to compare two strings.
 * If they are equal, the function shall return 1; otherwise 0.
 * @version 0.1
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int strs_are_equal(char *s1, char *s2);

/**
 * @brief Returns 1 if strings are equal, 0 otherwise
 *
 * @param s1
 * @param s2
 * @return int
 */
int strs_are_equal(char *s1, char *s2)
{
    return strcmp(s1, s2) == 0;
};

int main(void)
{
    char *s1 = "Test";
    char *s2 = "Test";
    char *s3 = "Test again";
    (void)printf("%d\n", strs_are_equal(s1, s2));
    (void)printf("%d\n", strs_are_equal(s1, s3));
    return 0;
};