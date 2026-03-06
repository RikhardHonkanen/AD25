/**
 * @file queue.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A multiple instance queue module implemented using a linked list to store data of type int.
 *        The module provides the following functions:
 *          1) A function to create a queue.
 *          2) A function to enqueue a data element to a queue.
 *          3) A function to dequeue a data element from a queue.
 *          4) A function to get the number of data elements stored in a queue.
 *          5) A function to destroy a queue.
 *
 * @version 0.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>

struct queue_t; // The forward declared ADT

/**
 * @brief This function is used to create a queue.
 *
 * @return queue_t* NULL if there is an error else a pointer to the created queue.
 */
queue_t *queue_create(void);

/**
 * @brief This function is used to enqueue a data element in a queue.
 *
 * @param queue Address of a queue.
 * @param value The data to enqueue.
 * @return bool false if there is an error else true.
 */
bool queue_write(queue_t *queue, int value);

/**
 * @brief This function is used to dequeue a data element from a queue.
 *
 * @param queue Address of a queue.
 * @param elem A reference to store the data
 * @return bool false if there is an error; otherwise true.
 */
bool queue_read(queue_t *queue, int &elem);

/**
 * @brief This function is used to get the number of data elements stored in a queue.
 *
 * @param queue Address of a queue.
 * @return size_t 0 if the queue is invalid or empty; otherwise, numer of stored elements.
 */
size_t queue_available(queue_t *queue);

/**
 * @brief This function is used to destroy a queue.
 *
 * @param queue A reference to a pointer to a queue.
 */
void queue_destroy(queue_t *&queue);

#endif