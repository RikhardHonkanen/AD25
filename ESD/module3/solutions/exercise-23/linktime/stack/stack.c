#include "stack.h"
#include "stdlib.h"

struct stack
{
    int top;
    int size;
    int *array;
};

stack_t *stack_create(int size)
{
    stack_t *ptr = NULL;

    if (size > 0)
    {
        ptr = (stack_t *)malloc(sizeof(stack_t));

        if (ptr != NULL)
        {
            ptr->array = (int *)malloc(size * sizeof(int));

            if (ptr->array != NULL)
            {
                ptr->size = size;
                ptr->top = -1;
            }
            else
            {
                free(ptr);
                ptr = NULL;
            }
        }
    }

    return ptr;
}

int stack_size(stack_t *stck)
{
    int size = 0;
    if (stck != NULL)
    {
        size = stck->size;
    }
    return size;
}

int stack_available(stack_t *stck)
{
    int available = -1;

    if (stck != NULL)
    {
        available = (stck->top + 1);
    }

    return available;
}

bool stack_clear(stack_t *stck)
{
    bool status = false;

    if (stck != NULL)
    {
        status = true;
        stck->top = -1;
    }

    return status;
}

bool stack_push(stack_t *stck, int data)
{
    bool status = false;

    if (stck != NULL)
    {
        if (stck->top < stck->size - 1)
        {
            stck->top++;
            status = true;
            stck->array[stck->top] = data;
        }
    }

    return status;
}

bool stack_pop(stack_t *stck, int *data)
{
    bool status = false;

    if ((stck != NULL) && (data != NULL))
    {
        if (stck->top > -1)
        {
            *data = stck->array[stck->top];
            status = true;
            stck->top--;
        }
    }

    return status;
}

void stack_destroy(stack_t **stck)
{
    if ((stck != NULL) && (*stck != NULL))
    {
        free((*stck)->array);
        free(*stck);
        *stck = NULL;
    }
}