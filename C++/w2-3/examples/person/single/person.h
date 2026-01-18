/**
 * @file person.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A single instance person module.
 * @version 0.1
 * @date 2021-11-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef PERSON_H
#define PERSON_H

#include <string>

namespace person
{
    /**
     * @brief This function is used to initialize the module
     *
     * @param pname Name of the person
     * @param page Age of the person
     */
    void init(const std::string &pname, int page);

    /**
     * @brief This function is used to change name of the person.
     *
     * @param pname The new name.
     */
    void set_name(const std::string &pname);

    /**
     * @brief This function is used to change age of the person.
     *
     * @param page The new age
     */
    void set_age(int page);

    /**
     * @brief This function is used to get name of the person.
     *
     * @return std::string Name of the person
     */
    std::string get_name(void);

    /**
     * @brief This function is used to get age of the person.
     *
     * @return int The person age.
     */
    int get_age(void);
}

#endif