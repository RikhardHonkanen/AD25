#include "stack.h"

stack *stack::handle(void)
{
    static stack stck;
    return &stck;
}

void stack::clear(void)
{
    top = -1;
}

int stack::available(void)
{
    return (top + 1);
}

bool stack::push(int value)
{
    bool status = false;

    if (top < (SIZE - 1))
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