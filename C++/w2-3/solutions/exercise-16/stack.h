/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a multiple instance stack using a fixed size array of type int.
 *        The stack shall be tested using assertion and provide the following functions:
 *          1. A function to create a stack.
 *          2. A function to push a data element a stack.
 *          3. A function to get number of the data elements stored in a stack.
 *          4. A function to pop an element from a stack.
 *          5. A function to clear a stack
 *          6. A function to destroy a stack

 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

struct stack_t; // The forward declared type

namespace stack
{

    constexpr int SIZE{8}; // Length of the array used as a stack

    /**
     * @brief This function is used to create a stack
     *
     * @return stack_t* nullptr if an error occurs else a pointer to the created stack.
     */
    stack_t *create(void);

    /**
     * @brief This function is used to insert an element into a stack.
     *
     * @param stck A pointer to the stack
     * @param value The data to push to the stack
     * @return bool false if the stack is full; otherwise true
     */
    bool push(stack_t *stck, int value);

    /**
     * @brief This function is used to get number of the element stored in a stack
     *
     * @param stck A pointer to the stack
     * @return int The number of available elements stored in the stack.
     */
    int available(stack_t *stck);

    /**
     * @brief This function is used to pop the top element from a stack
     *
     * @param stck A pointer to the stack
     * @param data A reference to placeholder to fill with the top element
     * @return bool - false if the stack is empty; otherwise true
     */
    bool pop(stack_t *stck, int &elem);

    /**
     * @brief This function is used to clear a stack
     *
     * @param stck A pointer to the stack
     */
    void clear(stack_t *stck);

    /**
     * @brief This function is used to destroy a stack
     *
     * @param stck A reference to a pointer to the stack
     */
    void destroy(stack_t *&stck);
}

#endif /* STACK_H */