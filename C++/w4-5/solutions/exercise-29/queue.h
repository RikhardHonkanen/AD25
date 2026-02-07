/**
 * @file queue.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make an only-movable template queue class
 *        using a linked list to provide the following functions:
 *          1. A function to enqueue a data element to a queue.
 *          2. A function to dequeue a data element from a queue.
 *          3. A function to get the number of data elements stored in a queue.
 *          4. A function to clear a queue.
 *
 *        - Use exception handling when you allocate memory dynamically.
 *        - Test the class using assertions and ensure that all the allocated
 *          memory blocks are released and the class is movable.
 *
 * @version 0.1
 * @date 2021-06-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <exception>

template <typename T>
class Queue
{
    struct node_t
    {
        T data;
        node_t *next;
        node_t(const T &_data, node_t *_next) : data{_data}, next{_next} {}
    };

    node_t *head{nullptr};
    node_t *tail{nullptr};
    size_t count{0};

public:
    Queue() = default;
    Queue(const Queue &) = delete;            // Uncopyable
    Queue &operator=(const Queue &) = delete; // Unassignable

    Queue(Queue &&queue) noexcept : head{queue.head}, tail{queue.tail}, count{queue.count}
    {
        // queue shall not own the resource
        queue.head = nullptr;
        queue.tail = nullptr;
        queue.count = 0;
    }

    Queue &operator=(Queue &&queue) noexcept
    {
        if (this != &queue)
        {
            clear(); // Release resources held by this instance

            // Move ownership of queue to this instance
            head = queue.head;
            tail = queue.tail;
            count = queue.count;

            // queue shall not own the resource
            queue.head = nullptr;
            queue.tail = nullptr;
            queue.count = 0;
        }

        return *this;
    }

    bool enqueue(const T &item) noexcept
    {
        bool status{true};

        try
        {
            node_t *node = new node_t{item, nullptr};

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

            count++;
        }
        catch (const std::bad_alloc &)
        {
            status = false;
        }

        return status;
    }

    bool dequeue(T &item) noexcept
    {
        bool status{false};

        if (head != nullptr)
        {
            status = true;
            item = head->data;

            node_t *temp{head};
            head = head->next;
            delete temp;
            count--;

            if (head == nullptr)
            {
                tail = nullptr;
            }
        }

        return status;
    }

    size_t available(void) noexcept
    {
        return count;
    }

    void clear(void) noexcept
    {
        while (head != nullptr)
        {
            node_t *temp{head};
            head = head->next;
            delete temp;
        }

        tail = head;
        count = 0;
    }

    ~Queue() { clear(); }
};

#endif /* QUEUE_H */