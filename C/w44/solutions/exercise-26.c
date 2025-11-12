/**
 * @file exercise-26.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a function named get_person to create an instance of person_t and then
 *        ask the user to enter values for the members in a terminal and
 *        in the end return the instance.
 *
 * @version 0.1
 * @date 2021-03-05
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>

#define NAME_LEN 8

#if NAME_LEN < 8
#error NAME_LEN shall be at least 8
#endif

typedef struct
{
    int age;
    float height;
    char name[NAME_LEN + 1];
} person_t;

// This function is used to clear the input buffer of the standard input.
static void clear_buffer(void)
{
    char chr = 0;
    while ((chr != '\n') && (chr != EOF))
    {
        chr = getchar();
    }
}

static person_t get_person(void)
{
    // Create an instance of person_t (person)
    person_t person = {0};

    // Using scanf read person.name from terminal
    (void)printf("Enter name(max. %u letters): ", NAME_LEN);

    char format[NAME_LEN] = {0};
    (void)sprintf(format, "%%%us", NAME_LEN); // %15s

    (void)scanf(format, person.name);

    // Using scanf to read person.age from terminal
    do
    {
        clear_buffer();
        (void)printf("Enter age: ");
    } while (1 != scanf("%d", &person.age));

    // Using scanf to read person.height from terminal
    do
    {
        clear_buffer();
        (void)printf("Enter height (m): ");
    } while (1 != scanf("%f", &person.height));

    return person;
}

int main(void)
{
    person_t eva = get_person();

    printf("\nAge: %d\n", eva.age);
    printf("Name: %s\n", eva.name);
    printf("Height: %gm\n", eva.height);

    return 0;
}