/**
 * @file queue.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A single instance queue module implemented using a linked list to store data of type int.
 *        The module provides the following functions:
 *          2) A function to enqueue a data element to the queue.
 *          3) A function to dequeue a data element from the queue.
 *          4) A function to get the number of data elements stored in the queue.
 *          5) A function to delete all the data elements stored in the queue.
 *
 * @version 0.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief This function is used to enqueue a data element in the queue.
 *
 * @param value The data to enqueue.
 * @return bool false if there is an error else true.
 */
bool queue_write(int value);

/**
 * @brief This function is used to dequeue a data element from the queue.
 *
 * @param data Address of a placeholder to store the data
 * @return bool false if there is an error; otherwise true.
 */
bool queue_read(int *data);

/**
 * @brief This function is used to get the number of data elements stored in the queue.
 *
 * @return size_t Numer of stored elements.
 */
size_t queue_available(void);

/**
 * @brief This function is used to delete all the data elements stored in the list.
 *
 */
void queue_clear(void);

#endif