/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Implement and test a stack class using an array of type int whose size is a compile-time constant.
 *        The class shall provide the following function:
 *          1. A function to push a data element to a stack.
 *          2. A function to get number of the data elements stored in a stack.
 *          3. A function to pop an element from a stack.
 *          4. A function to check if a stack is full or not.
 *          5. A function to check if a stack is empty or not.
 *          6. A function to clear a stack.
 *
 * @version 0.1
 * @date 2022-05-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

class stack
{
public:
    static constexpr int SIZE{10};

public:
    bool push(int value);

    bool pop(int &elem);

    int available(void);

    bool isempty(void);

    bool isfull(void);

    void clear(void);

private:
    int array[SIZE]{0};
    int top{-1};
};

#endif
