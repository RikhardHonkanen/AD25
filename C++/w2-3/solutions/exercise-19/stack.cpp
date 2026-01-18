#include "stack.h"

bool stack::push(int value)
{
    bool status = false;

    if (top < SIZE - 1)
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
        top--;
        status = true;
    }

    return status;
}

int stack::available(void)
{
    return (top + 1);
}

bool stack::isempty(void)
{
    return (top == -1);
}

bool stack::isfull(void)
{
    return (top == SIZE - 1);
}

void stack::clear(void)
{
    top = -1;
}
