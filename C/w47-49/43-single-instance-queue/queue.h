#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

/**
 * @brief
 * Add a value to the queue
 * @param value
 * @return true
 * @return false
 */
bool enqueue_value(int value);

/**
 * @brief
 * Remove a value from the queue
 * @param value
 * @return true
 * @return false
 */
bool dequeue_value(int value);

/**
 * @brief
 * Return no. of elements in the queue
 * @return int
 */
int queue_available(void);

/**
 * @brief
 * Remove all elements in the queue
 * @return int
 */
void queue_destroy(void);

#endif