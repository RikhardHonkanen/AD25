/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Implement and test an uncopyable singleton stack class using an array of type int
 *        whose size is a compile-time constant.
 *        The class shall provide the following function:
 *          1. A function to push a data element to the stack.
 *          2. A function to get number of the data elements stored in the stack.
 *          3. A function to pop an element from the stack.
 *          4. A function to clear the stack.

 * @version 0.1
 * @date 2022-05-13
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

private:
    int array[SIZE]{0};
    int top{-1};

    stack() = default;                        // Impossible to create instances
    stack(const stack &) = delete;            // Makes the class uncopyable
    stack &operator=(const stack &) = delete; // Makes the class unassignable

public:
    void clear(void);
    int available(void);
    bool pop(int &elem);
    bool push(int value);
    static stack *handle(void);
};

#endif
