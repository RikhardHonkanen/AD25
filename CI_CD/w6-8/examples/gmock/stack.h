/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An multiple stack module implemented using an array whose length is specified when it is created.
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

#include <cstddef>

struct IMemory
{
    virtual void *malloc(size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual ~IMemory() = default;
};

namespace stack
{
    struct stack_t; // The forward declared type

    /**
     * @brief This functio is used to initialize the module.
     *
     * @param imemory Address a class instance which has implemented IMemory
     */
    void init(IMemory *imemory);

    /**
     * @brief This function is used to create a stack
     *
     * @param size The capacity of the created stack. It shall be greater than 1
     * @return stack_t* stack_t* nullptr if an error occurs else a pointer to the created stack.
     */
    stack_t *create(int size);

    /**
     * @brief This function is used to insert an element into a stack.
     *
     * @param stck A pointer to the stack
     * @param value The data to push to the stack
     * @return bool false if the stack is full; otherwise true
     */
    bool push(stack_t *stck, int value);

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