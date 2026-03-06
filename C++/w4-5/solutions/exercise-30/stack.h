/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make an only-movable template stack class using an array whose size is
 *        specified during creation (size shall be greater than three)
 *        The class shall provide the following function:
 *          1. A function to push a data element to a stack.
 *          2. A function to get number of the data elements stored in a stack.
 *          3. A function to pop an element from a stack.
 *          4. A function to clear a stack
 *
 *        - Use exception handling, test the class using assertions,
 *          ensure that the dynamic memory is managed properly and the class is movable.
 *
 * @version 0.1
 * @date 2021-06-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class Stack
{
    int top{-1};
    int size{0};
    T *array{nullptr};
    static constexpr int SIZE_MIN{4};

public:
    Stack(const Stack &) = delete;            // Uncopyable
    Stack &operator=(const Stack &) = delete; // Unassignable

    Stack(int _size) : size{_size}
    {
        if (size < SIZE_MIN)
        {
            throw std::invalid_argument{"Invalid size"};
        }

        array = new T[size]{};
    }

    Stack(Stack &&stack) noexcept : top{stack.top}, size{stack.size}, array{stack.array}
    {
        // stack shall not own the resource
        stack.array = nullptr;
        stack.top = -1;
        stack.size = 0;
    }

    Stack &operator=(Stack &&stack) noexcept
    {
        if (this != &stack)
        {
            delete array; // Release the resource held by this instance

            // Move ownership of stack to this instance
            top = stack.top;
            size = stack.size;
            array = stack.array;

            // queue shall not own the resource
            stack.top = -1;
            stack.size = 0;
            stack.array = nullptr;
        }

        return *this;
    }

    bool push(const T &item) noexcept
    {
        bool status{false};

        if (top < (size - 1))
        {
            top++;
            status = true;
            array[top] = item;
        }

        return status;
    }

    bool pop(T &item) noexcept
    {
        bool status{false};

        if (top > -1)
        {
            item = array[top];
            status = true;
            top--;
        }

        return status;
    }

    int capacity(void) noexcept
    {
        return size;
    }

    int available(void) noexcept
    {
        return (top + 1);
    }

    void clear(void) noexcept
    {
        top = -1;
    }

    ~Stack() { delete array; }
};

#endif /* STACK_H */