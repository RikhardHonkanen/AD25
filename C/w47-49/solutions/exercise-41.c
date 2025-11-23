/**
 * @file exercise-41.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Write a program to make an array of person_t dynamically.
 *          1) The program shall ask the user to enter length of the array.
 *          2) Then the program shall allocate memory for the array.
 *          3) Then the program shall ask the user to enter name and age of each person.
 *                a) Memory for the name member shall be allocated dynamically.
 *          4) Then the program shall print all the persons to the output.
 *          5) In the end the program shall release all the dynamically allocated memory.
 *
 * @version 0.1
 * @date 2021-03-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX_LEN 32U

typedef struct
{
    char *name;
    unsigned int age;
} person_t;

static void clear_ibuffer(void)
{
    char ch = '\0';
    while ((ch != '\n') && (ch != EOF))
    {
        ch = (char)getchar();
    }
}

static void free_persons(size_t len, person_t **prsns)
{
    for (person_t *person = *prsns; (person - *prsns) < len; ++person)
    {
        if (person->name != NULL)
        {
            free(person->name);
            person->name = NULL;
        }
    }

    free(*prsns);
    *prsns = NULL;
}

int main(void)
{
    unsigned int length = 0;

    while (length < 1)
    {
        (void)printf("Enter length of the array: ");
        (void)scanf("%u", &length);
        clear_ibuffer();
    }

    person_t *persons = (person_t *)malloc(length * sizeof(person_t));

    if (persons == NULL)
    {
        (void)printf("Failed to allocate memory for persons.\n");
        exit(1);
    }

    for (unsigned int i = 0; i < length; ++i)
    {
        char format[NAME_MAX_LEN] = {0};
        char name[NAME_MAX_LEN + 1] = {0};

        (void)printf("Enter name of persons[%u]: ", i);
        (void)sprintf(format, "%%%us", NAME_MAX_LEN);
        (void)scanf(format, name);

        do
        {
            clear_ibuffer();
            (void)printf("Enter age of persons[%u]: ", i);
        } while (1 != scanf("%u", &persons[i].age));

        persons[i].name = (char *)calloc(strlen(name) + 1, sizeof(char));
        if (persons[i].name == NULL)
        {
            (void)printf("Failed to allocate memory for persons[%u].\n", i);
            free_persons(length, &persons);
            exit(1);
        }
        (void)strcpy(persons[i].name, name);
    }

    for (size_t i = 0; i < length; ++i)
    {
        (void)printf("%s\t%u\n", persons[i].name, persons[i].age);
    }

    free_persons(length, &persons);

    return 0;
}
