#include <new>
#include "queue.h"
#include <cassert>

struct node_t
{
    int data;
    node_t *next;
};

struct queue_t
{
    node_t *head;
    node_t *tail;
    size_t available;
};

queue_t *queue_create(void)
{
    return new (std::nothrow) queue_t{nullptr, nullptr, 0};
}

bool queue_write(queue_t *queue, int value)
{
    bool status = false;

    if (queue != nullptr)
    {
        node_t *new_node{new (std::nothrow) node_t{value, nullptr}};

        if (new_node != nullptr)
        {
            status = true;
            queue->available++;

            if (queue->head == nullptr)
            {
                queue->head = new_node;
                queue->tail = queue->head;
            }
            else
            {
                queue->tail->next = new_node;
                queue->tail = new_node;
            }
        }
    }

    return status;
}

bool queue_read(queue_t *queue, int &elem)
{
    bool status = false;

    if (queue != nullptr)
    {
        if (queue->head != nullptr)
        {
            status = true;
            queue->available--;
            elem = queue->head->data;
            node_t *head = queue->head;
            queue->head = queue->head->next;

            delete head;

            if (queue->head == nullptr)
            {
                queue->tail = nullptr;
            }
        }
    }

    return status;
}

size_t queue_available(queue_t *queue)
{
    assert(queue != nullptr);
    return queue->available;
}

void queue_destroy(queue_t *&queue)
{
    if ((queue != nullptr))
    {
        node_t *head = queue->head;

        while (head != nullptr)
        {
            node_t *current = head;
            head = head->next;
            delete current;
        }

        delete queue;
        queue = nullptr;
    }
}