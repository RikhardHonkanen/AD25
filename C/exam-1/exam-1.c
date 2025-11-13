/**
 * @file exam-1.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief
 * @version 0.1
 * @date 2025-11-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define NAME_LEN 31U
#define MAX_RECORDS 8U
#define AGE_MIN 5U
#define AGE_MAX 75U
#define FILENAME "students.bin"
#define STUDENT_FORMAT "%u: %u  %s\n"

typedef struct
{
    uint32_t id;
    uint8_t age;
    char name[NAME_LEN + 1];
} student_t;

bool write_students_to_file(size_t len, student_t *arr);
bool read_students_from_file(size_t len, student_t *arr);
bool create_student(uint8_t *id, student_t *arr);
void update_student(size_t len, uint8_t id, student_t *arr);
void delete_student(size_t len, uint8_t id, student_t *arr);
bool print_all_students(size_t len, student_t *arr);
bool print_single_student(size_t len, uint8_t id, student_t *arr);
void print_student_header(void);
void print_main_menu(void);
int find_next_id(size_t len, student_t *arr);
void flush_buffer(void);

void flush_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // flush bad input
}

int find_next_id(size_t len, student_t *arr)
{
    int highest = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (arr[i].id > highest)
        {
            highest = arr[i].id;
        }
    }

    return highest + 1;
}

bool print_single_student(size_t len, uint8_t id, student_t *arr)
{
    bool status = false;

    if ((arr != NULL) && (id > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            if (arr[i].id == id)
            {
                print_student_header();
                (void)printf(STUDENT_FORMAT, arr[i].id, arr[i].age, arr[i].name);
            }
        }
    }
    return status;
}

bool print_all_students(size_t len, student_t *arr)
{
    printf("\n");
    print_student_header();
    bool status = false;

    if ((arr != NULL) && (len > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            if (arr[i].id == 0)
            {
                continue;
            }

            (void)printf(STUDENT_FORMAT, arr[i].id, arr[i].age, arr[i].name);
        }
    }

    return status;
}

bool create_student(uint8_t *id, student_t *arr)
{
    bool status = true;
    uint8_t age = 0;
    char name[NAME_LEN] = {0};

    do
    {
        (void)printf("Enter student age: ");
        (void)scanf("%d", &age);
    } while (age < 5 || age > 75);
    flush_buffer();
    do
    {
        (void)printf("Enter student name: ");
        (void)fgets(name, sizeof(name), stdin);
        int len = strlen(name);
        if (name[len - 1] == '\n')
        {
            len--;
            name[len] = '\0';
        }
    } while (strlen(name) == 0);

    student_t new_student = {.id = *id, .age = age};
    for (size_t j = 0; j < NAME_LEN; j++)
    {
        new_student.name[j] = name[j];
    }
    arr[(*id) - 1] = new_student;
    (*id)++;

    FILE *file = fopen(FILENAME, "rb+");

    if (file == NULL)
    {
        status = false;
    }
    else
    {
        if (1 != fwrite(arr, sizeof(student_t), 1, file))
        {
            status = false;
        }

        (void)fclose(file);
    }

    return status;
}

void update_student(size_t len, uint8_t id, student_t *arr)
{
    bool status = false;

    if ((arr != NULL) && (id > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            if (arr[i].id == id)
            {
                uint8_t age = 0;
                char name[NAME_LEN] = {0};
                do
                {
                    (void)printf("Enter new student age: ");
                    (void)scanf("%d", &age);
                } while (age < 5 || age > 75);
                flush_buffer();
                do
                {
                    (void)printf("Enter new student name: ");
                    (void)fgets(name, sizeof(name), stdin);
                    int len = strlen(name);
                    if (name[len - 1] == '\n')
                    {
                        len--;
                        name[len] = '\0';
                    }
                } while (strlen(name) == 0);

                arr[i].age = age;
                for (size_t j = 0; j < NAME_LEN; j++)
                {
                    arr[i].name[j] = name[j];
                }
            }
        }
    }
    return status;
}

void delete_student(size_t len, uint8_t id, student_t *arr)
{
    bool status = false;

    if ((arr != NULL) && (id > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            if (arr[i].id == id)
            {
                student_t blank = {0};
                arr[i] = blank;
            }
        }
    }
    return status;
}

void print_student_header(void)
{
    printf("ID  Age Name\n");
    printf("============================\n");
}

void print_main_menu(void)
{
    printf("Welcome to StudentDB(tm)!\n");
    printf("============================\n");
    printf("Options:\n");
    printf("A: Print all students\n");
    printf("C: Create new student\n");
    printf("P: Print a single student\n");
    printf("E: Edit existing student\n");
    printf("D: Delete a student\n");
    printf("Q: Quit StudentDB(tm)\n");
}

bool write_students_to_file(size_t len, student_t *arr)
{
    bool status = true;
    if ((arr == NULL) || (len == 0))
    {
        status = false;
    }
    else
    {
        FILE *file = fopen(FILENAME, "wb");

        if (file == NULL)
        {
            status = false;
        }
        else
        {
            if (len != fwrite(arr, sizeof(student_t), len, file))
            {
                status = false;
            }

            (void)fclose(file);
        }
    }

    return status;
}

bool read_students_from_file(size_t len, student_t *arr)
{
    bool status = true;

    if ((arr == NULL) || (len == 0))
    {
        status = false;
    }
    else if (NULL != fopen(FILENAME, "wbx"))
    {
        // Create file if not exists
        printf("Created students file!\n");
    }
    else
    {
        FILE *file = fopen(FILENAME, "rb");

        if (file == NULL)
        {
            status = false;
        }
        else
        {
            (void)memset(arr, 0, len * sizeof(student_t));

            if (len != fread(arr, sizeof(student_t), len, file))
            {
                status = false;
            }

            (void)fclose(file);
        }
    }

    return status;
}

// TODO: (BUG) figure out why first record (id: 1) gets set to id: 0 after call to update or delete functions
// The record still exists in file but will no longer get printed due to logic in program ignoring id: 0
int main(void)
{
    uint8_t id = 1;
    student_t students[MAX_RECORDS] = {0};
    char option = ' ';

    if (!read_students_from_file(MAX_RECORDS, students))
    {
        (void)printf("Failed to read students!\n");
        exit(1);
    }

    id = find_next_id(sizeof(students) / sizeof(student_t), students);

    while (true)
    {
        printf("\n");
        print_main_menu();
        do
        {
            option = getchar();
        } while (option != 'a' && option != 'A' && option != 'c' && option != 'C' && option != 'p' && option != 'P' && option != 'e' && option != 'E' && option != 'q' && option != 'Q' && option != 'd' && option != 'D');

        if (option == 'c' || option == 'C')
        {
            create_student(&id, students);
            write_students_to_file(sizeof(students) / sizeof(student_t), students);
        }
        if (option == 'e' || option == 'E')
        {
            uint8_t single_id = 0;
            do
            {
                (void)printf("Enter id: ");
                (void)scanf("%d", &single_id);
            } while (single_id <= 0 || single_id > MAX_RECORDS);
            update_student(sizeof(students) / sizeof(student_t), single_id, students);
            write_students_to_file(sizeof(students) / sizeof(student_t), students);
        }
        else if (option == 'a' || option == 'A')
        {
            print_all_students(sizeof(students) / sizeof(student_t), students);
        }
        else if (option == 'p' || option == 'P')
        {
            uint8_t single_id = 0;
            do
            {
                (void)printf("Enter id: ");
                (void)scanf("%d", &single_id);
            } while (single_id <= 0 || single_id > MAX_RECORDS);
            print_single_student(sizeof(students) / sizeof(student_t), single_id, students);
        }
        else if (option == 'd' || option == 'D')
        {
            uint8_t single_id = 0;
            do
            {
                (void)printf("Enter id: ");
                (void)scanf("%d", &single_id);
            } while (single_id <= 0 || single_id > MAX_RECORDS);
            delete_student(sizeof(students) / sizeof(student_t), single_id, students);
            write_students_to_file(sizeof(students) / sizeof(student_t), students);
        }
        else if (option == 'q' || option == 'Q')
        {
            break;
        }
        option = ' ';
    }

    return 0;
};