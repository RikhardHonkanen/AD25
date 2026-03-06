/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Implement an uncopyable stack class using a linked list.
 *        The stack shall be tested using assertions and provide the following functions:
 *          1) A function to push a data element a stack.
 *          2) A function to get number of the data elements stored in a stack.
 *          3) A function to pop an element from a stack.
 *          4) If the template type is an arithmetic type then
 *             there shall be a function to return average of the data elements stored in a stack.
 *          5) A function to clear a stack.
 *
 * @version 0.1
 * @date 2022-11-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

#include <new>
#include <cstddef>
#include <type_traits>

template <typename T>
class stack
{
    struct node
    {
        T data{};
        node *next;
        node(const T &value, node *ptr) : data{value}, next{ptr} {}
    };

    node *top{nullptr};
    size_t elem_nums{0};

public:
    stack() = default;
    stack(const stack &) = delete;
    stack &operator=(const stack &) = delete;

    bool push(const T &elem)
    {
        bool status{true};

        try
        {
            top = new node{elem, top};
            elem_nums++;
        }
        catch (std::bad_alloc)
        {
            status = false;
        }

        return status;
    }

    bool pop(T &elem)
    {
        bool status{false};

        if (top != nullptr)
        {
            status = true;
            elem = top->data;

            node *temp = top;
            top = top->next;
            delete temp;
            elem_nums--;
        }

        return status;
    }

    size_t available(void)
    {
        return elem_nums;
    }

    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, float> average(void)
    {
        float avg{0};
        node *cursor{top};

        while (cursor != nullptr)
        {
            avg += cursor->data;
            cursor = cursor->next;
        }

        return (elem_nums == 0) ? avg : (avg / elem_nums);
    }

    void clear(void)
    {
        while (top != nullptr)
        {
            node *cursor = top;
            top = top->next;
            delete cursor;
        }
        elem_nums = 0;
    }

    ~stack() { clear(); }
};

#endif /* STACK_H */
