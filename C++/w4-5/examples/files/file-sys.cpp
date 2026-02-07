/**
 * @file file-sys.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of file handling using POSIX syscall
 *        1) Make an array(users) of user_t
 *        2) Make a function to fill users with random names and unique ids started from 1
 *        3) Make a function to print the users
 *        4) Make a function to write users to a binary file
 *        5) Make a function to read the users from the file
 *        6) Make a function to change a user name base on its id
 *        7) Use the POSIX syscall for file handling (https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/)
 *
 * @version 0.1
 * @date 2021-03-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <ctime>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

constexpr int USERS{4};
constexpr int NAME_LEN{8};
constexpr int ALPHABETS{26};
const char *FILENAME{"users.bin"};

struct user_t
{
    uint32_t id;
    char name[NAME_LEN + 1];
};

static bool fill_users(size_t len, user_t *arr);
static bool print_users(size_t len, const user_t *arr);
static bool write_users(size_t len, const user_t *arr);
static bool read_users(size_t len, user_t *arr);
static bool update_user(uint32_t id, const char *name);

int main(void)
{
    user_t users[USERS]{0};

    if (!fill_users(USERS, users))
    {
        std::cout << "Failed to fill the array!" << std::endl;
        exit(1);
    }

    if (!print_users(USERS, users))
    {
        std::cout << "Failed to print users!" << std::endl;
        exit(1);
    }

    if (!write_users(USERS, users))
    {
        std::cout << "Failed to write users to the file!" << std::endl;
        exit(1);
    }

    if (!read_users(USERS, users))
    {
        std::cout << "Failed to read users from the file!" << std::endl;
        exit(1);
    }

    std::cout << std::endl;

    if (!print_users(USERS, users))
    {
        std::cout << "Failed to print users!" << std::endl;
        exit(1);
    }

    if (!update_user(3, "Abcdefgh"))
    {
        std::cout << "Failed to update the user!" << std::endl;
        exit(1);
    }

    if (!read_users(USERS, users))
    {
        std::cout << "Failed to read users from the file!" << std::endl;
        exit(1);
    }

    std::cout << std::endl;

    if (!print_users(USERS, users))
    {
        std::cout << "Failed to print users!" << std::endl;
        exit(1);
    }

    return 0;
}

static bool update_user(uint32_t id, const char *name)
{
    bool status{true};
    if ((name == nullptr) || (id == 0))
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
            user_t user{0};

            while (sizeof(user_t) == read(file, &user, sizeof(user_t)))
            {
                if (user.id == id)
                {
                    (void)std::strncpy(user.name, name, NAME_LEN);
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
    bool status{true};
    if ((arr == nullptr) || (len == 0))
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
            (void)std::memset(arr, 0, len * sizeof(user_t));

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
    bool status{true};
    if ((arr == nullptr) || (len == 0))
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
    bool status{false};

    if ((arr != nullptr) && (len > 0))
    {
        status = true;
        std::srand(std::time(nullptr));

        for (size_t i = 0; i < len; i++)
        {
            arr[i].id = i + 1;
            arr[i].name[0] = 'A' + std::rand() % ALPHABETS;
            for (size_t j = 1; j < NAME_LEN; j++)
            {
                arr[i].name[j] = 'a' + std::rand() % ALPHABETS;
            }
        }
    }

    return status;
}

static bool print_users(size_t len, const user_t *arr)
{
    bool status{false};

    if ((arr != nullptr) && (len > 0))
    {
        status = true;
        for (size_t i = 0; i < len; i++)
        {
            std::cout << arr[i].id << ": " << arr[i].name << std::endl;
        }
    }

    return status;
}
