/**
 * @file 41.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Write a program to make an array of person_t dynamically.
 * The program shall ask the user to enter length of the array.
 * Then the program shall allocate memory for the array.
 * Then the program shall ask the user to enter name and age of each person.
 * Memory for the name member shall be allocated dynamically.
 * Then the program shall print all the persons to the output.
 *  In the end the program shall release all the dynamically allocated memory.
 * @version 0.1
 * @date 2025-11-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAME_LEN 31

typedef struct
{
    char *name;
    uint8_t age;
} person_t;

void flush_buffer(void);
static bool print_the_people(size_t len, person_t *people);
static person_t create_person(int idx);

int main(void)
{
    int arr_size;
    (void)printf("Enter number of people: ");
    (void)scanf("%d", &arr_size);
    person_t *the_people = (person_t *)malloc(arr_size * sizeof(person_t));

    for (size_t i = 0; i < arr_size; i++)
    {
        the_people[i] = create_person(i);
    }

    if (!print_the_people(arr_size, the_people))
    {
        (void)printf("Failed to print the people!");
        exit(1);
    }

    for (size_t i = 0; i < arr_size; i++)
    {
        free(the_people[i].name);
    }
    free(the_people);

    return 0;
}

static person_t create_person(int idx)
{
    person_t p;
    char buffer[255];

    printf("Enter age of person %d: ", idx + 1);
    scanf("%hhu", &p.age);
    flush_buffer();

    printf("Enter person name: ");
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = '\0';

    p.name = malloc(strlen(buffer) + 1);
    if (!p.name)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    strcpy(p.name, buffer);

    return p;
}

static bool print_the_people(size_t len, person_t *people)
{
    (void)printf("\n");
    (void)printf("The people:\n");
    bool status = false;

    if ((people != NULL) && (len > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            (void)printf("Age: %d, name: %s\n", people[i].age, people[i].name);
        }
    }

    return status;
}

void flush_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // flush bad input
}