#include "queue.h"
#include <assert.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node_t;

static node_t *head = NULL;
static node_t *tail = NULL;
static size_t available = 0;

bool queue_write(int value)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node != NULL)
    {
        available++;
        new_node->next = NULL;
        new_node->data = value;

        if (head == NULL)
        {
            head = new_node;
            tail = head;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
    }

    return (new_node != NULL);
}

bool queue_read(int *data)
{
    bool status = false;

    if ((head != NULL) && (data != NULL))
    {
        available--;
        status = true;
        *data = head->data;

        node_t *temp = head;
        head = head->next;
        free(temp);

        if (head == NULL)
        {
            tail = NULL;
        }
    }

    return status;
}

size_t queue_available(void)
{
    return available;
}

void queue_clear(void)
{
    while (head != NULL)
    {
        node_t *current = head;
        head = head->next;
        free(current);
    }
    available = 0;
    tail = head;
}