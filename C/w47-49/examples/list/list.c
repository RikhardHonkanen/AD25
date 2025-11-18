#include "list.h"
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node_t;

static node_t *head = NULL;
static size_t available = 0;

bool list_insert(int value)
{
    bool status = false;

    node_t *new_node = (node_t *)malloc(sizeof(node_t)); // Create a node

    if (new_node != NULL)
    {
        available++;
        status = true;
        new_node->next = NULL;
        new_node->data = value;

        if (head == NULL) // The list is empty
        {
            head = new_node;
        }
        else // Insert the node int the end of the list
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

size_t list_available(void)
{
    return available;
}

bool list_search(int value)
{
    bool status = false;
    node_t *cursor = head;

    while (cursor != NULL)
    {
        if (cursor->data == value)
        {
            status = true;
            break;
        }

        cursor = cursor->next;
    }

    return status;
}

bool list_remove(int value)
{
    bool status = false;
    node_t *current = head;
    node_t *previous = NULL;

    while (current != NULL)
    {
        if (current->data == value)
        {
            available--;
            status = true;

            if (previous == NULL)
            {
                head = current->next;
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

void list_clear(void)
{
    while (head != NULL)
    {
        node_t *next = head->next;
        free(head);
        head = next;
    }
    available = 0;
}