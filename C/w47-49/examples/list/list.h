/**
 * @file list.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A single instance linked list module to store data of type int.
 *        The module shall provide:
 *          1) A function to insert a new data element into the linked list
 *          2) A function to get number of the data elements stored in the linked list
 *          3) A function to search for a specific value
 *          4) A function to remove a specific value in the linked list
 *          5) A function to delete all the nodes in the linked list
 *
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief This function is used to insert a data element into the list.
 *
 * @param value The integer to insert.
 * @return bool - true if value is inserted successfully; otherwise false
 */
bool list_insert(int value);

/**
 * @brief This function returns number of the data elements stored in the list.
 *
 * @return size_t - Number of the data elements
 */
size_t list_available(void);

/**
 * @brief This function is used to search for a specific data in the list.
 *
 * @param value The data to search for.
 * @return bool - true if the value is in the list; otherwise false.
 */
bool list_search(int value);

/**
 * @brief This function is used to delete the nth node in the list
 *
 * @param value The data to remove from the list
 * @return bool - false if the value is not in the list; otherwise true
 */
bool list_remove(int value);

/**
 * @brief This function is used to free the allocated memory for all the nodes.
 *
 */
void list_clear(void);

#endif