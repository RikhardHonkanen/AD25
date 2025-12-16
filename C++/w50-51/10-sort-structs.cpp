/**
 * @file 10.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a struct type named person_t which has, name, age and id members.
 * Make a program to create an array of 3 persons. Then read 3 persons from the standard input and then
 * sort the persons according thier ids and print the persons to the standard output.
 * To sort and print use only pointers.
 * @version 0.1
 * @date 2025-12-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

constexpr int MAX_PERSONS{3};

struct Person
{
    std::string name;
    int age;
    int id;
};
// using person_t = Person;

Person create_person(void)
{
    std::string name;
    int age;
    int id;

    std::cout << "Enter person name:\n";
    std::cin >> name;
    std::cout << "Enter person age:\n";
    std::cin >> age;
    std::cout << "Enter person id:\n";
    std::cin >> id;

    return {name, age, id};
}

int main(void)
{
    std::vector<Person> persons{};

    for (int i{0}; i < MAX_PERSONS; i++)
    {
        persons.push_back(create_person());
    }

    std::sort(persons.begin(), persons.end(),
              [](const Person &a, const Person &b)
              { return a.id < b.id; });
    for (auto const p : persons)
    {
        std::cout << "ID: " << p.id << '\t' << p.name << ", " << p.age << '\n';
    }

    return 0;
};