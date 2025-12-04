/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Implement a single instance stack module using a linked list to store data of type int.
 * @version 0.1
 * @date 2021-11-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief This function is used to clear the stack.
 *
 */
void stack_clear(void);

/**
 * @brief This function is used to push an integer to the stack.
 *
 * @param value The data to push
 * @return bool true if value is pushed successfully; otherwise false.
 */
bool stack_push(int value);

/**
 * @brief
 *
 * @return int The data popped from the stack.
 */

/**
 * @brief This function is used to pop the top element from the stack.
 *
 * @param data A pointer to a placeholder to store the popped data.
 * @return bool false if an error occurs else true.
 */
bool stack_pop(int *data);

/**
 * @brief This function is used to get the number of elements on the stack.
 *
 * @return size_t The number of elements on the stack.
 */
size_t stack_available(void);

#endif
