/**
 * @file person.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A single instance person module.
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PERSON_H
#define PERSON_H

#include <stdint.h>

#define PERSON_NAME_MAX_LEN 32 // Shall be an integer in the range of 8 and 64

/**
 * @brief This function is used to initialize the module
 *
 * @param _name Name of the person
 * @param _age Age of the person
 */
void person_init(const char *_name, uint8_t _age);

/**
 * @brief This function is used to change name of the person.
 *
 * @param _name The new name.
 */
void person_set_name(const char *_name);

/**
 * @brief This function is used to change age of the person.
 *
 * @param _age The new age
 */
void person_set_age(uint8_t _age);

/**
 * @brief This function is used to get name of the person.
 *
 * @param _name The person name.
 */
void person_get_name(char *_name);

/**
 * @brief This function is used to get age of the person.
 *
 * @return uint8_t The person age.
 */
uint8_t person_get_age(void);

#endif