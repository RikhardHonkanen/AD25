/**
 * @file 26.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a function named get_person, to create an instance of person_t and then
 * ask the user to enter values for the members in a terminal and
 * in the end return the instance.
 * @version 0.1
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

typedef struct
{
    int age;
    float height;
    char name[16];
} person_t;

person_t get_person(void);

person_t get_person(void)
{
    person_t person;
    (void)printf("Enter person age: ");
    scanf(" %d", &person.age);
    (void)printf("Enter person height: ");
    scanf(" %f", &person.height);
    (void)printf("Enter person name: ");
    scanf(" %s", &person.name);

    return person;
};

int main(void)
{
    person_t person = get_person();
    (void)printf("Age: %d, height: %f, name: %s\n", person.age, person.height, person.name);

    return 0;
};