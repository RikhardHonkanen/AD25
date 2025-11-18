#include "queue.h"
#include <assert.h>
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
    size_t available;
};

queue_t *queue_create(void)
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));

    if (queue != NULL)
    {
        queue->head = NULL;
        queue->tail = NULL;
        queue->available = 0;
    }

    return queue;
}

bool queue_write(queue_t *queue, int value)
{
    bool status = false;

    if (queue != NULL)
    {
        node_t *new_node = (node_t *)malloc(sizeof(node_t));

        if (new_node != NULL)
        {
            status = true;
            queue->available++;
            new_node->next = NULL;
            new_node->data = value;

            if (queue->head == NULL)
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

bool queue_read(queue_t *queue, int *iptr)
{
    bool status = false;

    if ((queue != NULL) && (iptr != NULL))
    {
        if (queue->head != NULL)
        {
            status = true;
            queue->available--;
            *iptr = queue->head->data;
            node_t *head = queue->head;
            queue->head = queue->head->next;

            free(head);

            if (queue->head == NULL)
            {
                queue->tail = NULL;
            }
        }
    }

    return status;
}

size_t queue_available(queue_t *queue)
{
    assert(queue != NULL);
    return queue->available;
}

void queue_destroy(queue_t **queue)
{
    if ((queue != NULL) && (*queue != NULL))
    {
        node_t *head = (*queue)->head;

        while (head != NULL)
        {
            node_t *current = head;
            head = head->next;
            free(current);
        }

        free(*queue);
        *queue = NULL;
    }
}