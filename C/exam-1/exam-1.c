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

bool create_student(uint32_t *id);
bool update_student(uint32_t id);
bool delete_student(uint32_t id);
bool print_all_students(void);
bool print_single_student(uint32_t id);
void print_student_header(void);
void print_main_menu(void);
uint32_t find_next_id(void);
void flush_buffer(void);

void flush_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // flush bad input
}

uint32_t find_next_id(void)
{
    uint32_t next_id = 1; // default if no valid records
    FILE *file = fopen(FILENAME, "rb");
    if (file != NULL)
    {
        if (fseek(file, 0, SEEK_END) == 0)
        {
            long file_size = ftell(file);
            long record_count = file_size / sizeof(student_t);

            for (long i = record_count - 1; i >= 0; i--)
            {
                if (fseek(file, i * sizeof(student_t), SEEK_SET) == 0)
                {
                    student_t temp;
                    if (fread(&temp, sizeof(student_t), 1, file) == 1)
                    {
                        if (temp.id != 0)
                        {
                            next_id = temp.id + 1;
                            break;
                        }
                    }
                }
            }
        }
        fclose(file);
    }
    return next_id;
}

bool print_single_student(uint32_t id_to_print)
{
    bool status = false;

    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL)
    {
        status = false;
    }
    else
    {
        student_t temp;
        while (fread(&temp, sizeof(student_t), 1, file) == 1)
        {
            if ((temp.id == id_to_print) && (temp.id != 0))
            {
                print_student_header();
                (void)printf(STUDENT_FORMAT, temp.id, temp.age, temp.name);
                status = true;
                break;
            }
        }
        fclose(file);
    }
    return status;
}

bool print_all_students(void)
{
    bool status = false;

    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL)
    {
        status = false;
    }
    else
    {
        student_t temp;
        print_student_header();

        while (fread(&temp, sizeof(student_t), 1, file) == 1)
        {
            if (temp.id != 0) // skip logically deleted records
            {
                (void)printf(STUDENT_FORMAT, temp.id, temp.age, temp.name);
                status = true;
            }
        }
        fclose(file);
    }
    return status;
}

bool create_student(uint32_t *id)
{
    bool status = true;

    if (id == NULL)
    {
        status = false;
    }
    else
    {
        student_t new_student = {0};
        new_student.id = *id;

        do
        {
            (void)printf("Enter student age (%u-%u): ", AGE_MIN, AGE_MAX);
            if (scanf("%hhu", &new_student.age) != 1)
            {
                flush_buffer();
                new_student.age = 0;
            }
            else
            {
                flush_buffer();
            }
        } while (new_student.age < AGE_MIN || new_student.age > AGE_MAX);

        do
        {
            (void)printf("Enter student name: ");
            if (fgets(new_student.name, sizeof(new_student.name), stdin) == NULL)
            {
                status = false;
            }
            else
            {
                size_t len = strlen(new_student.name);
                if (len > 0 && new_student.name[len - 1] == '\n')
                {
                    new_student.name[len - 1] = '\0';
                }
            }
        } while ((status == true) && (strlen(new_student.name) == 0));

        if (status == true)
        {
            FILE *file = fopen(FILENAME, "ab");
            if (file == NULL)
            {
                status = false;
            }
            else
            {
                if (fwrite(&new_student, sizeof(student_t), 1, file) != 1)
                {
                    status = false;
                }
                fclose(file);
            }
        }

        if (status == true)
        {
            (*id)++;
        }
    }
    return status;
}

bool update_student(uint32_t id_to_update)
{
    bool status = false;

    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        status = false;
    }
    else
    {
        student_t temp;
        while (fread(&temp, sizeof(student_t), 1, file) == 1)
        {
            if ((temp.id == id_to_update) && (temp.id != 0))
            {
                uint8_t new_age = 0;
                do
                {
                    (void)printf("Enter new student age (%u-%u): ", AGE_MIN, AGE_MAX);
                    if (scanf("%hhu", &new_age) != 1)
                    {
                        flush_buffer();
                        new_age = 0;
                    }
                    else
                    {
                        flush_buffer();
                    }
                } while (new_age < AGE_MIN || new_age > AGE_MAX);
                temp.age = new_age;

                do
                {
                    (void)printf("Enter new student name: ");
                    if (fgets(temp.name, sizeof(temp.name), stdin) == NULL)
                    {
                        status = false;
                    }
                    else
                    {
                        status = true;
                        size_t len = strlen(temp.name);
                        if ((len > 0) && (temp.name[len - 1] == '\n'))
                        {
                            temp.name[len - 1] = '\0';
                        }
                    }
                } while ((status == false) || (strlen(temp.name) == 0));

                if (status == true)
                {
                    if (fseek(file, -((long)sizeof(student_t)), SEEK_CUR) == 0)
                    {
                        if (fwrite(&temp, sizeof(student_t), 1, file) == 1)
                        {
                            status = true; // success
                        }
                        else
                        {
                            status = false;
                        }
                    }
                    else
                    {
                        status = false;
                    }
                }
                break;
            }
        }
        fclose(file);
    }
    return status;
}

bool delete_student(uint32_t id_to_delete)
{
    bool status = false;

    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        status = false;
    }
    else
    {
        student_t temp;
        while (fread(&temp, sizeof(student_t), 1, file) == 1)
        {
            if (temp.id == id_to_delete)
            {
                student_t blank = {0}; // Insert zeroed record in place of data
                if (fseek(file, -((long)sizeof(student_t)), SEEK_CUR) == 0)
                {
                    if (fwrite(&blank, sizeof(student_t), 1, file) == 1)
                    {
                        status = true;
                    }
                }
                break;
            }
        }

        fclose(file);
    }

    return status;
}

void print_student_header(void)
{
    (void)printf("\n");
    (void)printf("ID  Age Name\n");
    (void)printf("============================\n");
}

void print_main_menu(void)
{
    (void)printf("Welcome to StudentDB(tm)!\n");
    (void)printf("============================\n");
    (void)printf("Options:\n");
    (void)printf("A: Print all students\n");
    (void)printf("C: Create new student\n");
    (void)printf("P: Print a single student\n");
    (void)printf("E: Edit existing student\n");
    (void)printf("D: Delete a student\n");
    (void)printf("Q: Quit StudentDB(tm)\n");
}

int main(void)
{
    uint32_t id;
    char option = ' ';

    id = find_next_id();

    while (true)
    {
        (void)printf("\n");
        print_main_menu();
        do
        {
            option = getchar();
        } while (option != 'a' && option != 'A' && option != 'c' && option != 'C' && option != 'p' && option != 'P' && option != 'e' && option != 'E' && option != 'q' && option != 'Q' && option != 'd' && option != 'D');

        if (option == 'c' || option == 'C')
        {
            if (!create_student(&id))
            {
                (void)printf("Failed to create student!\n");
            }
        }
        if (option == 'e' || option == 'E')
        {
            uint32_t single_id = 0;
            do
            {
                (void)printf("Enter id: ");
                (void)scanf("%d", &single_id);
                flush_buffer();
            } while (single_id <= 0 || single_id > UINT32_MAX);
            if (!update_student(single_id))
            {
                (void)printf("Failed to update student!\n");
            }
        }
        else if (option == 'a' || option == 'A')
        {
            if (!print_all_students())
            {
                (void)printf("Failed to print students!\n");
            }
        }
        else if (option == 'p' || option == 'P')
        {
            uint32_t single_id = 0;
            do
            {
                (void)printf("Enter id: ");
                (void)scanf("%d", &single_id);
                flush_buffer();
            } while (single_id <= 0 || single_id > UINT32_MAX);
            if (!print_single_student(single_id))
            {
                (void)printf("Failed to print student!\n");
            }
        }
        else if (option == 'd' || option == 'D')
        {
            uint32_t single_id = 0;
            do
            {
                (void)printf("Enter id: ");
                (void)scanf("%d", &single_id);
                flush_buffer();
            } while (single_id <= 0 || single_id > UINT32_MAX);
            if (!delete_student(single_id))
            {
                (void)printf("Failed to delete student!\n");
            }
        }
        else if (option == 'q' || option == 'Q')
        {
            break;
        }
        option = ' ';
    }

    return 0;
};