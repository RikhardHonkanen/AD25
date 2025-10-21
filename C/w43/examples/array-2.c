/**
 * @file example.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of array:
 *        1. Write a program to make a 2 element array of type person_t
 *        2. Initialize the array with the following info
 *            a. The first person:
 *                 - id: 100
 *                 - age: 23
 *                 - birthdate: 1990-10-10
 *            b. The second person:
 *                 - id: 200
 *                 - age: 32
 *                 - birthdate: 1980-11-21
 *        3. Print the persons in the array to the terminal
 *
 *        Example output:
 *           Person 1:
 *              - Id: 100
 *              - Age: 23
 *              - Birthdate: 1990-10-10
 *           Person 2:
 *              - Id: 200
 *              - Age: 32
 *              - Birthdate: 1980-11-21
 *
 * @version 0.1
 * @date 2022-10-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

#define PERSONS 2

typedef struct
{
    int id, age;
    struct
    {
        int day;
        int month;
        int year;
    } birthdate;
} person_t;

int main(void)
{
    person_t persons[PERSONS] = {
        {.id = 100, .age = 23, .birthdate = {.day = 10, .month = 10, .year = 1990}},
        {.id = 200, .age = 32, .birthdate = {.day = 21, .month = 11, .year = 1980}}};

    for (int i = 0; i < PERSONS; i++)
    {
        (void)printf("Person %d:\n", i + 1);
        (void)printf("   - Id: %d\n", persons[i].id);
        (void)printf("   - Age: %d\n", persons[i].age);
        (void)printf("   - Birthdate: %04d-%02d-%02d\n",
                     persons[i].birthdate.year, persons[i].birthdate.month, persons[i].birthdate.day);
    }

    return 0;
}