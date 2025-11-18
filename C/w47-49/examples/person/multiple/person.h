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

#include <stdint.h>

#define PERSON_NAME_MAX_LEN 32 // Shall be an integer in the range of 8 and 64

typedef struct person person_t; // A forward declared type

/**
 * @brief This function is used to create a person.
 *
 * @param name Name of the person
 * @param age Age of the person
 * @return person_t* NULL if an error occurs else a pointer to the created person.
 */
person_t *person_create(const char *name, uint8_t age);

/**
 * @brief This function is used to change name of a person.
 *
 * @param prsn A pointer to the person.
 * @param name The new name.
 */
void person_set_name(person_t *prsn, const char *name);

/**
 * @brief This function is used to change age of a person.
 *
 * @param prsn A pointer to the person.
 * @param age The new age.
 */
void person_set_age(person_t *prsn, uint8_t age);

/**
 * @brief This function is used to get name of a person.
 *
 * @param prsn A pointer to the person.
 * @param name The person name.
 */
void person_get_name(person_t *prsn, char *name);

/**
 * @brief This function is used to get age of a person.
 *
 * @param prsn A pointer to the person.
 * @return uint8_t The person age.
 */
uint8_t person_get_age(person_t *prsn);

/**
 * @brief This function is used to destroy a person.
 *
 * @param prsn Address of a pointer to the person.
 */
void person_destroy(person_t **prsn);

#endif