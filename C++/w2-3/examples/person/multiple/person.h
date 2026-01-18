/**
 * @file person.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A multiple instance person module.
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PERSON_H
#define PERSON_H

#include <string>

struct person_t; // A forward declared type

namespace person
{
    /**
     * @brief This function is used to create a person.
     *
     * @param name Name of the person
     * @param age Age of the person
     * @return person_t* nullptr if an error occurs else a pointer to the created person.
     */
    person_t *create(const std::string &name, int age);

    /**
     * @brief This function is used to change name of a person.
     *
     * @param prsn A pointer to the person.
     * @param name The new name.
     */
    void set_name(person_t *prsn, const std::string &name);

    /**
     * @brief This function is used to change age of a person.
     *
     * @param prsn A pointer to the person.
     * @param age The new age.
     */
    void set_age(person_t *prsn, int age);

    /**
     * @brief This function is used to get name of a person.
     *
     * @param prsn A pointer to the person.
     * @return std::string Name of the person
     */
    std::string get_name(person_t *prsn);

    /**
     * @brief This function is used to get age of a person.
     *
     * @param prsn A pointer to the person.
     * @return int The person age.
     */
    int get_age(person_t *prsn);

    /**
     * @brief This function is used to destroy a person.
     *
     * @param prsn Refernce to a pointer to the person.
     */
    void destroy(person_t *&prsn);
}

#endif