#include "queue.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct node
{
    int32_t data;
    struct node *next;

} node_t;

static node_t *head = NULL;
static size_t available = 0;

bool enqueue_value(int value)
{
    bool status = false;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node != NULL)
    {
        available++;
        status = true;
        new_node->next = NULL;
        new_node->data = value;

        if (head == NULL) // Empty list
        {
            head = new_node;
        }
        else
        {
            node_t *current = head;
            while (current->next != NULL)
            {
                current = current->next;
            }

            current->next = new_node;
        }
    }

    return status;
}
bool dequeue_value(int value)
{
    bool status = false;
    node_t *current = head;
    node_t *previous = NULL;

    while (current != NULL)
    {
        if (current->data == value)
        {
            status = true;
            available--;
            if (previous == NULL)
            {
                head->next = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            break;
        }

        previous = current;
        current = current->next;
    }

    return status;
}
int queue_available(void)
{
    return available;
}
void queue_destroy(void)
{
    while (head != NULL)
    {
        node_t *next = head->next;
        free(head);
        head = next;
    }
    available = 0;
}