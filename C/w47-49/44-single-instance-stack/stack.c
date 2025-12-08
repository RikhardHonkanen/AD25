#include "stack.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct node
{
    int32_t data;
    struct node *next;
} node_t;

static node_t *head = NULL;
static int32_t available = 0;

bool stack_push(int value)
{
    bool status = false;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node != NULL)
    {
        status = true;
        available++;
        new_node->next = head;
        new_node->data = value;
        head = new_node;
    }

    return status;
}

bool stack_pop(int *value)
{
    bool status = false;

    if (head != NULL)
    {
        status = true;
        *value = head->data;
        node_t *next = head->next; // Can be NULL, empty stack
        free(head);
        available--;
        head = next;
    }

    return status;
}

int stack_available(void)
{
    return available;
}

void stack_destroy(void)
{
    while (head != NULL)
    {
        node_t *next = head->next;
        free(head);
        head = next;
    }
    available = 0;
}
