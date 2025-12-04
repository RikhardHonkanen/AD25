/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Implement a multiple instance stack using an array whose length is specified when an instance is created.
 *        The stack shall be tested using assertions and provide the following functionalities:
 *           1) A function to create an instance. Length of the array used by the instance is a parameter of this function.
 *           2) A function to push a data element of type uint8_t to a stack.
 *           3) A function to get number of the data elements stored in the stack.
 *           4) A function to pop an element from a stack.
 *           5) A function to get length of a stack
 *           6) A function to clear a stack
 *           7) A function to destroy a stack
 *
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct stack stack_t; // stack_t is a forward declared abstracted data type

/**
 * @brief This function is used to create a stack
 *
 * @param size Length of the array used by the stack
 * @return stack_t* - NULL if an error occurs else a pointer to the created stack.
 */
stack_t *stack_create(uint16_t size);

/**
 * @brief This function returns length of the array used by a stack
 *
 * @param stck A pointer to the stack
 * @return uint16_t - 0 if stck is invalid; otherwise size of the stack
 */
uint16_t stack_size(stack_t *stck);

/**
 * @brief This function is used to clear a stack
 *
 * @param stck A pointer to the stack
 * @return bool - false if stck is not valid; otherwise true
 */
bool stack_clear(stack_t *stck);

/**
 * @brief This function returns the number of available data elements stored in the stack
 *
 * @param stck A pointer to the stack
 * @return int32_t -1 if the stck is invalid; otherwise the number of data elements stored in the stack.
 */
int32_t stack_available(stack_t *stck);

/**
 * @brief This function is used to insert an element in the stack.
 *
 * @param stck A pointer to the stack
 * @param data The data to push to the stack
 * @return bool - false if the stack is invalid or full; otherwise true
 */
bool stack_push(stack_t *stck, uint8_t data);

/**
 * @brief This function is used to pop the top element in the stack
 *
 * @param stck A pointer to the stack
 * @param data A pointer to a placeholder to fill with the top element
 * @return bool - false if the stack is empty or stck or data are invalid; otherwise true
 */
bool stack_pop(stack_t *stck, uint8_t *data);

/**
 * @brief This function is used to destroy a stack
 *
 * @param stck Address of a pointer to the stack
 */
void stack_destroy(stack_t **stck);

#endif /* STACK_H */
