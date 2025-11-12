/**
 * @file exercise-39.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of file handling in C
 *        1) Make an array(users) of user_t
 *        2) Make a function to fill users with random names and unique ids started from 1
 *        3) Make a function to print the users
 *        4) Make a function to write users to a binary file
 *        5) Make a function to read the users from the file
 *        6) Make a function to change a user name base on its id
 *        7) Use the POSIX syscall for file handling (https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/)
 * @version 0.1
 * @date 2021-03-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define USERS 4U
#define NAME_LEN 8U
#define ALPHABETS 26
#define FILENAME "users.bin"
#define USER_FORMAT "%u: %s\n"

typedef struct
{
    uint32_t id;
    char name[NAME_LEN + 1];
} user_t;

static bool fill_users(size_t len, user_t *arr);
static bool print_users(size_t len, const user_t *arr);
static bool write_users(size_t len, const user_t *arr);
static bool read_users(size_t len, user_t *arr);
static bool update_user(uint32_t id, const char *name);

int main(void)
{
    user_t users[USERS] = {0};

    if (!fill_users(USERS, users))
    {
        (void)printf("Failed to fill the array!\n");
        exit(1);
    }

    if (!print_users(USERS, users))
    {
        (void)printf("Failed to print users!\n");
        exit(1);
    }

    if (!write_users(USERS, users))
    {
        (void)printf("Failed to write users to the file!\n");
        exit(1);
    }

    if (!read_users(USERS, users))
    {
        (void)printf("Failed to read users from the file!\n");
        exit(1);
    }

    (void)printf("\n");

    if (!print_users(USERS, users))
    {
        (void)printf("Failed to print users!\n");
        exit(1);
    }

    if (!update_user(3, "Abcdefgh"))
    {
        (void)printf("Failed to update the user!\n");
        exit(1);
    }

    if (!read_users(USERS, users))
    {
        (void)printf("Failed to read users from the file!\n");
        exit(1);
    }

    (void)printf("\n");

    if (!print_users(USERS, users))
    {
        (void)printf("Failed to print users!\n");
        exit(1);
    }

    return 0;
}

static bool update_user(uint32_t id, const char *name)
{
    bool status = true;
    if ((name == NULL) || (id == 0))
    {
        status = false;
    }
    else
    {
        int file = open(FILENAME, O_RDWR);

        if (file < 0)
        {
            status = false;
        }
        else
        {
            user_t user = {0};

            while (read(file, &user, sizeof(user_t)) == sizeof(user_t))
            {
                if (user.id == id)
                {
                    (void)strncpy(user.name, name, NAME_LEN);
                    if (-1 == lseek(file, lseek(file, 0, SEEK_CUR) - sizeof(user_t), SEEK_SET))
                    {
                        status = false;
                    }
                    else
                    {
                        if (sizeof(user_t) != write(file, &user, sizeof(user_t)))
                        {
                            status = false;
                        }
                    }

                    break;
                }
            }

            (void)close(file);
        }
    }
    return status;
}

static bool read_users(size_t len, user_t *arr)
{
    bool status = true;
    if ((arr == NULL) || (len == 0))
    {
        status = false;
    }
    else
    {
        int file = open(FILENAME, O_RDONLY);

        if (file < 0)
        {
            status = false;
        }
        else
        {
            (void)memset(arr, 0, len * sizeof(user_t));

            for (size_t i = 0; i < len; i++)
            {
                if (sizeof(user_t) != read(file, &arr[i], sizeof(user_t)))
                {
                    status = false;
                    break;
                }
            }
            (void)close(file);
        }
    }

    return status;
}

static bool write_users(size_t len, const user_t *arr)
{
    bool status = true;
    if ((arr == NULL) || (len == 0))
    {
        status = false;
    }
    else
    {
        int file = open(FILENAME, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

        if (file < 0)
        {
            status = false;
        }
        else
        {
            uint32_t length = len * sizeof(user_t);
            if (length != (uint32_t)write(file, arr, length))
            {
                status = false;
            }

            (void)close(file);
        }
    }

    return status;
}

static bool fill_users(size_t len, user_t *arr)
{
    bool status = false;

    if ((arr != NULL) && (len > 0))
    {
        status = true;
        srand(time(NULL));

        for (size_t i = 0; i < len; i++)
        {
            arr[i].id = (i + 1);
            for (size_t j = 0; j < NAME_LEN; j++)
            {
                arr[i].name[j] = ((j == 0) ? 'A' : 'a') + rand() % ALPHABETS;
            }
        }
    }

    return status;
}

static bool print_users(size_t len, const user_t *arr)
{
    bool status = false;

    if ((arr != NULL) && (len > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            (void)printf(USER_FORMAT, arr[i].id, arr[i].name);
        }
    }

    return status;
}
