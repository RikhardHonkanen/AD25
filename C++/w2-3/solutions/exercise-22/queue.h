/**
 * @file queue.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make an uncopyable queue class using a linked list to store and restore data of type int and
 *        to provide the following functions:
 *          1. A function to enqueue a data element to a queue.
 *          2. A function to dequeue a data element from a queue.
 *          3. A function to get the number of data elements stored in a queue.
 *          4. A function to clear a queue.
 *
 *        Test the class using assertions and ensure that
 *        all the allocated memory blocks are released
 *
 * @version 0.1
 * @date 2021-06-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <new>
#include <cstddef>

class queue
{
    struct node_t
    {
        int data;
        node_t *next;
        node_t(int d, node_t *ptr) : data{d}, next{ptr} {}
    };

    node_t *head{nullptr};
    node_t *tail{nullptr};
    size_t elems_num{0};

public:
    queue() = default;
    queue(const queue &) = delete;
    queue &operator=(const queue &) = delete;

    bool enqueue(const int &value)
    {
        bool status{false};

        node_t *node = new (std::nothrow) node_t{value, nullptr};

        if (node != nullptr)
        {
            if (tail == nullptr)
            {
                head = node;
                tail = head;
            }
            else
            {
                tail->next = node;
                tail = node;
            }

            status = true;
            elems_num++;
        }

        return status;
    }

    bool dequeue(int &value)
    {
        bool status{false};

        if (head != nullptr)
        {
            status = true;
            value = head->data;

            node_t *temp{head};
            head = head->next;
            delete temp;
            elems_num--;

            if (head == nullptr)
            {
                tail = nullptr;
            }
        }

        return status;
    }

    size_t available(void)
    {
        return elems_num;
    }

    void clear(void)
    {
        while (head != nullptr)
        {
            node_t *temp{head};
            head = head->next;
            delete temp;
        }
        elems_num = 0;
        tail = head;
    }

    ~queue() { clear(); }
};

#endif /* QUEUE_H */