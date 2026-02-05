#ifndef STACK_H
#define STACK_H

#include <cstddef>

struct IMemory
{
    virtual void *malloc(size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual ~IMemory() = default;
};

class Stack
{
    struct node_t
    {
        int data;
        node_t *next;
        node_t(int d, node_t *ptr) : data{d}, next{ptr} {}
    };

    size_t count{0};
    node_t *top{nullptr};
    IMemory *memory{nullptr};

public:
    Stack(const Stack &) = delete;
    Stack(IMemory *imemory) : memory{imemory} {};
    Stack &operator=(const Stack &) = delete;

    bool push(int value)
    {
        bool status{false};

        if (memory != nullptr)
        {
            node_t *node = reinterpret_cast<node_t *>(memory->malloc(sizeof(node_t)));

            if (node != nullptr)
            {
                node->data = value;
                node->next = top;
                top = node;

                status = true;
                count++;
            }
        }

        return status;
    }

    bool pop(int &value)
    {
        bool status{false};

        if ((memory != nullptr) && (top != nullptr))
        {
            status = true;
            value = top->data;
            node_t *temp{top};
            top = top->next;
            memory->free(temp);
            count--;
        }

        return status;
    }

    size_t available(void)
    {
        return count;
    }

    void clear(void)
    {
        while (top != nullptr)
        {
            node_t *temp{top};
            top = top->next;
            memory->free(temp);
        }
        count = 0;
    }

    ~Stack() { clear(); }
};

#endif /* STACK_H */