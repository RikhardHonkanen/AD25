#include "stack.h"
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node_t;

static node_t *top = NULL;

void stack_clear(void)
{
    node_t *current = top;

    while (current != NULL)
    {
        top = top->next;
        free(current);
        current = top;
    }
}

bool stack_push(int value)
{
    bool status = false;
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node != NULL)
    {
        new_node->data = value;
        new_node->next = top;
        top = new_node;
        status = true;
    }

    return status;
}

bool stack_pop(int *data)
{
    bool status = false;

    if ((top != NULL) && data != NULL)
    {
        status = true;
        *data = top->data;
        node_t *temp = top;
        top = top->next;
        free(temp);
    }

    return status;
}

size_t stack_available(void)
{
    size_t count = 0;
    node_t *current = top;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}
