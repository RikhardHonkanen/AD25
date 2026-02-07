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
    struct node_t
    {
        T data{};
        node_t *next;
        node_t(const T &_data, node_t *_next) : data{_data}, next{_next} {}
    };

    size_t elem_nums{0};
    node_t *top{nullptr};

public:
    stack() = default;
    stack(const stack &) = delete;
    stack &operator=(const stack &) = delete;

    bool push(const T &elem)
    {
        bool status{false};

        node_t *node = new (std::nothrow) node_t{elem, top};

        if (node != nullptr)
        {
            top = node;
            elem_nums++;
            status = true;
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

            node_t *temp = top;
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

    void clear(void)
    {
        while (top != nullptr)
        {
            node_t *cursor = top;
            top = top->next;
            delete cursor;
        }
        elem_nums = 0;
    }

    ~stack() { clear(); }
};

#endif /* STACK_H */
