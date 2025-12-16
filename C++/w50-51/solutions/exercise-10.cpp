/**
 * @file exercise-10.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a struct type named person_t which has, name, age and id members.
 *        Make a program to create an array of 3 persons. Then read 3 persons from the
 *        standard input and then sort the persons according thier ids and print the persons
 *        to the standard output. To sort and print use only pointers.
 *
 * @version 0.1
 * @date 2021-02-27
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <ios>
#include <limits>
#include <string>
#include <iostream>

struct person_t
{
    int id;
    int age;
    std::string name;
};

constexpr int PERSON_NUM{3};

int main(void)
{
    person_t persons[PERSON_NUM];

    for (auto &elem : persons)
    {
        std::cout << "Enter name: ";
        std::cin >> elem.name;

        elem.age = -1;
        do
        {
            std::cout << "Enter age: ";
            std::cin >> elem.age;
            if (std::cin.fail())
            {
                elem.age = -1;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        } while (elem.age == -1);

        elem.id = -1;
        do
        {
            std::cout << "Enter ID: ";
            std::cin >> elem.id;
            if (std::cin.fail())
            {
                elem.id = -1;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        } while (elem.id == -1);

        std::cout << std::endl;
    }

    for (person_t *iptr{persons}; iptr - persons < PERSON_NUM; iptr++)
    {
        for (person_t *jptr{iptr + 1}; jptr - persons < PERSON_NUM; jptr++)
        {
            if (iptr->id > jptr->id)
            {
                person_t temp{*iptr};
                *iptr = *jptr;
                *jptr = temp;
            }
        }
    }

    for (person_t *ptr{persons}; ptr - persons < PERSON_NUM; ptr++)
    {
        std::cout << ptr->id << "\t" << ptr->name << "\t" << ptr->age << std::endl;
    }

    std::cout << std::endl;

    return 0;
}