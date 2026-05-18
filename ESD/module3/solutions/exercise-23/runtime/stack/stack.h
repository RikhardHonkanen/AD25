#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    void *(*allocate)(size_t);
    void (*release)(void *);
} stack_memory_t;

typedef struct stack stack_t; // stack_t is a forward declared abstract data type

/**
 * @brief This function is used to initialize the module
 *
 * @param ptr A pointer to an instance of stack_memory_t.
 */
void stack_init(const stack_memory_t *ptr);

/**
 * @brief This function is used to create a stack
 *
 * @param size The capacity of the stack.
 * @return stack_t* - NULL if an error occurres; otherwis a pointer to the created stack.
 */
stack_t *stack_create(int size);

/**
 * @brief This function returns size of the stack
 *
 * @param stck A pointer to the stack
 * @return int - 0 if stck is invalid; otherwise size of the stack
 */
int stack_size(stack_t *stck);

/**
 * @brief This function is used to clear a stack
 *
 * @param stck A pointer to the stack
 * @return bool - false if stck is not valid; otherwise true
 */
bool stack_clear(stack_t *stck);

/**
 * @brief This function returns the number of available data elements in the stack
 *
 * @param stck A pointer to the stack
 * @return int -1 if the stck is invalid; otherwise the number of data elements to pop
 */
int stack_available(stack_t *stck);

/**
 * @brief This function is used to insert an element in the stack.
 *
 * @param stck A pointer to the stack
 * @param data The data to push to the stack
 * @return bool - false if the stack is full; otherwise true
 */
bool stack_push(stack_t *stck, int data);

/**
 * @brief This function is used to pop the top element in the stack
 *
 * @param stck A pointer to the stack
 * @param data A pointer to an placeholder to fill with the top element
 * @return bool - false if the stack is empty; otherwise true
 */
bool stack_pop(stack_t *stck, int *data);

/**
 * @brief This function is used to destroy a stack
 *
 * @param stck Address of a pointer to the stack
 */
void stack_destroy(stack_t **stck);

#endif