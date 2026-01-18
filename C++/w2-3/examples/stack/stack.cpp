#include "stack.h"

static_assert((STACK_SIZE >= 4) && (STACK_SIZE <= 1024), "STACK_SIZE shall be an integer in the range of 4 and 1024");

static int top = -1;
static int array[STACK_SIZE] = {0};

void stack::clear(void)
{
    top = -1;
}

bool stack::push(int value)
{
    bool status = false;

    if (top < (STACK_SIZE - 1))
    {
        top++;
        status = true;
        array[top] = value;
    }

    return status;
}

bool stack::pop(int &elem)
{
    bool status = false;

    if (top > -1)
    {
        elem = array[top];
        status = true;
        top--;
    }

    return status;
}

int stack::available(void)
{
    return (top + 1);
}
