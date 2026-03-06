#include <cstdlib>
#include "stack.h"

struct stack::stack_t
{
    int top;
    int size;
    int *array;
};

static IMemory *memory{nullptr};

void stack::init(IMemory *imemory)
{
    memory = imemory;
}

stack::stack_t *stack::create(int size)
{
    stack_t *ptr{nullptr};

    if ((memory != nullptr) && (size > 1))
    {
        ptr = reinterpret_cast<stack_t *>(memory->malloc(sizeof(stack_t)));

        if (ptr != nullptr)
        {
            ptr->top = -1;
            ptr->size = size;

            ptr->array = reinterpret_cast<int *>(memory->malloc(size * sizeof(int)));

            if (ptr->array == nullptr)
            {
                memory->free(ptr);
                ptr = nullptr;
            }
        }
    }

    return ptr;
}

bool stack::push(stack_t *stck, int value)
{
    bool status{false};

    if (stck != nullptr)
    {
        if (stck->top < stck->size - 1)
        {
            stck->top++;
            status = true;
            stck->array[stck->top] = value;
        }
    }

    return status;
}

int stack::available(stack_t *stck)
{
    int num = -1;

    if (stck != nullptr)
    {
        num = stck->top + 1;
    }

    return num;
}

bool stack::pop(stack_t *stck, int &elem)
{
    bool status{false};

    if (stck != nullptr)
    {
        if (stck->top > -1)
        {
            elem = stck->array[stck->top];
            status = true;
            stck->top--;
        }
    }

    return status;
}

void stack::clear(stack_t *stck)
{
    if (stck != nullptr)
    {
        stck->top = -1;
    }
}

void stack::destroy(stack_t *&stck)
{
    if ((memory != nullptr) && (stck != nullptr))
    {
        memory->free(stck->array);
        memory->free(stck);
        stck = nullptr;
    }
}