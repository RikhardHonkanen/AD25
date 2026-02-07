/**
 * @file file-bin.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of file handling in C++
 *        1) Make an array(users) of user_t
 *        2) Make a function to fill users with random names and unique ids started from 1
 *        3) Make a function to print the users
 *        4) Make a function to write users to a binary file
 *        5) Make a function to read the users from the file
 *        6) Make a function to change a user name base on its id
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
#include <fstream>
#include <iostream>

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
        std::fstream file(FILENAME, std::ios::binary | std::ios::in | std::ios::out);

        if (!file.is_open())
        {
            status = false;
        }
        else
        {
            while (!file.eof())
            {
                user_t user{0};

                std::streampos pos = file.tellg();

                file.read(reinterpret_cast<char *>(&user), sizeof(user_t));

                if (file.bad())
                {
                    status = false;
                    break;
                }

                if (user.id == id)
                {
                    (void)std::strncpy(user.name, name, NAME_LEN);

                    file.seekp(pos);

                    file.write(reinterpret_cast<char *>(&user), sizeof(user_t));

                    if (file.bad())
                    {
                        status = false;
                    }

                    break;
                }
            }

            file.close();
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
        std::ifstream file(FILENAME, std::ios::binary);

        if (!file.is_open())
        {
            status = false;
        }
        else
        {
            (void)std::memset(arr, 0, len * sizeof(user_t));

            file.read(reinterpret_cast<char *>(arr), len * sizeof(user_t));

            if (file.bad())
            {
                status = false;
            }

            file.close();
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
        std::ofstream file(FILENAME, std::ios::binary);

        if (!file.is_open())
        {
            status = false;
        }
        else
        {
            file.write(reinterpret_cast<char *>(const_cast<user_t *>(arr)), len * sizeof(user_t));

            if (file.bad())
            {
                status = false;
            }

            file.close();
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
