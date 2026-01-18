/**
 * @file stack.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Implement and test a stack class using a linked list to store and restore data of type int.
 *        Test the class using assertions and ensure that all the allocated memory blocks are released.
 *        The class shall provide the following function:
 *          1. A function to push a data element to a stack.
 *          2. A function to get number of the data elements stored in a stack.
 *          3. A function to pop an element from a stack.
 *          4. A function to clear a stack.
 *          5. Overload the insertion operator to print a stack to the terminal

 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef STACK_H
#define STACK_H

#include <iostream>

class Stack
{
    struct node_t
    {
        int data;
        node_t *next;
        node_t(int _data, node_t *ptr) : data{_data}, next{ptr} {}
    };

    node_t *top{nullptr};
    size_t elems_num{0};

public:
    Stack() = default;
    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

    bool push(int value)
    {
        bool status{false};

        node_t *node = new (std::nothrow) node_t{value, top};

        if (node != nullptr)
        {
            status = true;
            elems_num++;
            top = node;
        }

        return status;
    }

    bool pop(int &elem)
    {
        bool status{false};

        if (top != nullptr)
        {
            status = true;
            elem = top->data;
            node_t *temp{top};
            top = top->next;
            delete temp;
            elems_num--;
        }

        return status;
    }

    size_t available(void)
    {
        return elems_num;
    }

    void clear(void)
    {
        while (top != nullptr)
        {
            node_t *temp{top};
            top = top->next;
            delete temp;
        }

        elems_num = 0;
    }

    friend std::ostream &operator<<(std::ostream &out, const Stack &stack)
    {
        node_t *cursor{stack.top};

        while (cursor != nullptr)
        {
            out << cursor->data << "\t";
            cursor = cursor->next;
        }

        return out;
    }

    ~Stack() { clear(); }
};

#endif