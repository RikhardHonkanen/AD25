/**
 * @file util.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief
 * @version 0.1
 * @date 2025-12-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "util.h"
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node_t;

struct queue
{
    node_t *head;
    node_t *tail;
    size_t count;
    size_t size;
};

queue_t *queue_create(int size)
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));

    if (queue != NULL && size > 2) // Return empty queue if size too small
    {
        queue->head = NULL;
        queue->tail = NULL;
        queue->count = 0;
        queue->size = size;

        node_t *previous = NULL;
        for (size_t i = 0; i < size; i++)
        {
            node_t *current = (node_t *)malloc(sizeof(node_t));
            current->data = NULL;
            if (NULL == queue->head) // First node
            {
                queue->head = current;
                queue->tail = current;
            }
            if (i == size - 1) // Last node
            {
                current->next = queue->head;
            }
            if (NULL != previous)
            {
                previous->next = current;
            }

            previous = current;
        }
    }

    return queue;
}

bool queue_resize(queue_t *queue, int new_size)
{
    bool status = false;

    if (queue != NULL)
    {
        status = true;

        if (new_size > queue->size)
        {
            node_t *current = queue->tail;

            while (current->next != queue->head) // Go to last node (connecting to head)
            {
                current = current->next;
            }
            for (size_t i = 0; i < new_size - queue->size; i++)
            {
                node_t *new_current = (node_t *)malloc(sizeof(node_t));
                new_current->data = NULL;
                current->next = new_current;
                current = new_current;

                if (i == (new_size - queue->size - 1)) // Last insert
                {
                    current->next = queue->head;
                }
            }
            queue->size = new_size;
        }
        else if (new_size < queue->size)
        {
            int removed = 0;
            if (queue_full(queue))
            {
                node_t *current = queue->head;

                for (size_t i = 0; i < queue->size - new_size; i++)
                {
                    node_t *next = current->next;
                    queue->head = current->next;
                    free(current);
                    current = queue->head;
                }

                queue->tail->next = current;
                queue->count = new_size;
                queue->size = new_size;
            }
            else
            {
                node_t *current = queue->tail;

                for (size_t i = 0; i < queue->size - new_size; i++)
                {
                    if (current->next->data == NULL)
                    {
                        node_t *new_next = current->next->next;
                        free(current->next);
                        current->next = new_next;
                    }
                    else // Data present, move head
                    {
                        queue->head = queue->head->next;
                        free(current->next);
                        current->next = queue->head;
                    }
                }
                queue->size = new_size;
            }
        }
    }

    return status;
}

bool queue_insert(queue_t *queue, int value)
{
    bool status = false;

    if (queue != NULL)
    {
        status = true;
        if (queue->tail->next == queue->head) // Queue full
        {
            queue->tail = queue->head;
            queue->head = queue->head->next;
            queue->head->data = value;
        }
        else
        {
            (queue->count)++;
            queue->tail->data = value;
            queue->tail = queue->tail->next;
        }
    }

    return status;
}

bool dequeue(queue_t *queue, int *value)
{
    bool status = false;

    if (queue != NULL && queue->head->data != NULL)
    {
        status = true;
        *value = queue->head->data;
        queue->head->data = NULL;
        queue->head = queue->head->next;
        (queue->count)--;
    }

    return status;
}

bool queue_empty(queue_t *queue)
{
    bool status = false;

    if (queue != NULL)
    {
        status = true;
        node_t *current = queue->head;

        while (current->data != NULL)
        {
            current->data = NULL;
            current = current->next;
        }

        queue->count = 0;
        queue->head = current;
        queue->tail = current;
    }

    return status;
}

ssize_t queue_count(queue_t *queue)
{
    ssize_t count = -1;

    if (queue != NULL)
    {
        count = queue->count;
    }

    return count;
}

bool queue_full(queue_t *queue)
{
    return queue->count == queue->size;
}

ssize_t queue_size(queue_t *queue)
{
    ssize_t size = -1;

    if (queue != NULL)
    {
        size = queue->size;
    }

    return size;
}

void queue_destroy(queue_t **queue)
{
    if ((queue != NULL) && (*queue != NULL))
    {
        node_t *current = (*queue)->head;
        node_t *start = (*queue)->head;

        while (current != NULL)
        {
            (*queue)->head = current->next;
            free(current);
            if ((*queue)->head == start)
            {
                break;
            }
            else
            {
                current = (*queue)->head;
            }
        }

        free(*queue);
        *queue = NULL;
    }
}