#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/**
 * @brief
 * Push a value to the top of the stack
 * @param value
 * @return true
 * @return false
 */
bool stack_push(int value);

/**
 * @brief
 * Pop a value from the top of the stack
 * @param value pointer to store the popped element
 * @return true if successful
 * @return false otherwise
 */
bool stack_pop(int *value);

/**
 * @brief
 * Return no. of elements on the stack
 * @return int
 */
int stack_available(void);

/**
 * @brief
 * Remove all elements in the stack
 */
void stack_destroy(void);

#endif