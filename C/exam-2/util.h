/**
 * @file util.h
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Develop and test a multiple instance circular queue implemented using a circular singly linked list to store and restore data of type
 * int according to the requirements bellow:
 * The module shall have a create function to create an instance
 * Size of a queue is passed to this function and it shall be greater than 2
 * The nodes shall be created and linked in this function
 * The module shall have a function to write data into a queue
 *
 * If the queue is full then the oldest data shall be overwritten
 * No node shall be created or deleted
 *
 * The module shall have a function to read data from a queue
 * No node shall be created or deleted
 *
 * The module shall have a function to make a queue empty
 * No node shall be created or deleted
 *
 * The module shall have a function to get size of a queue
 * The module shall have a function to return number of data elements stored in a queue
 * The module shall have a function to check if a queue is full or not
 * The module shall have a function to resize a queue
 * No queue shall be created
 * The new size shall also be greater than 2
 * In the case you need to delete node(s) which hold data element(s), nodes holding the oldest data element(s) shall be deleted.
 * The module shall have a destroy function to release the allocated memory for an instance
 * The dynamic memory shall be managed properly
 * The module shall be tested using assertion
 * @version 0.1
 * @date 2025-12-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <sys/types.h>

typedef struct queue queue_t;

/**
 * @brief This function is used to create a linked circular queue.
 *
 * @param size Number of elements in queue
 * @return queue_t*
 */
queue_t *queue_create(int size);

/**
 * @brief This function is used to resize en existing queue
 *
 * @param queue
 * @param new_size
 * @return bool - false if an error occurs, true otherwise.
 */
bool queue_resize(queue_t *queue, int new_size);

/**
 * @brief This function is used to insert a data element into a queue.
 *
 * @param queue A pointer to the queue.
 * @param value The data to insert.
 * @return bool - false if an error occurs; otherwise true
 */
bool queue_insert(queue_t *queue, int value);

/**
 * @brief This function is used to dequeue a value.
 *
 * @param queue A pointer to the queue.
 * @param value A pointer to store dequeued value.
 * @return bool - false if no value or error occurs, true otherwise
 */
bool dequeue(queue_t *queue, int *value);

/**
 * @brief This function is used to empty all values from a queue
 *
 * @param queue A pointer to the queue.
 * @return bool - true on success, false otherwise.
 */
bool queue_empty(queue_t *queue);

/**
 * @brief This function returns number of the data elements stored in a queue.
 *
 * @param queue A pointer to the queue.
 * @return ssize_t: -1 if the queue is empty or queue is NULL else number of the data elements
 */
ssize_t queue_count(queue_t *queue);

/**
 * @brief This function checks if queue is full.
 *
 * @param queue A pointer to the queue.
 * @return bool - true if queue is full, false otherwise
 */
bool queue_full(queue_t *queue);

/**
 * @brief This function returns the size of a queue.
 *
 * @param queue A pointer to the queue.
 * @return ssize_t: -1 if the queue is NULL else size of queue
 */
ssize_t queue_size(queue_t *queue);

/**
 * @brief This function is used to destroy a queue.
 *
 * @param queue Address of a pointer to a queue to destroy.
 */
void queue_destroy(queue_t **queue);

#endif