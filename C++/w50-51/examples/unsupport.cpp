/**
 * @file unsupport.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief Some unsupported features of C by C++
 *        Try to compile the code by running: g++ -pedantic-errors unsupport.cpp
 * @version 0.1
 * @date 2024-11-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    int age;
    char name[32];
} person_t;

void print(person_t *person)
{
    printf("%s's ID is %d and she is %d years old.\n", person->name, person->id, person->age);
}

void func(int len)
{
    int array[len];

    memset(array, 0, sizeof(array));

    for (int i = 0; i < len; i++)
    {
        array[i] = i;
    }

    for (int i = 0; i < len; i++)
    {
        printf("%d\t", array[i]);
    }

    printf("\n");
}

int main(void)
{
    int array[5] = {[2] = 100};
    int *list = (int[]){1, 2, 3}; // (int[3]){1, 2, 3};
    person_t eva = {.id = 1000, .age = 23, "Eva"};

    printf("array[0] = %d, array[2] = %d\n", array[0], array[2]);
    printf("list[0] = %d, list[1] = %d, list[2] = %d\n", list[0], list[1], list[2]);

    print(&eva);
    print(&(person_t){2000, .age = 30, "Linda"});

    int length;
    printf("Enter length: ");
    (void)scanf("%d", &length);
    func(length);

    return 0;
}