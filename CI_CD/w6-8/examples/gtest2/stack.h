#ifndef STACK_H
#define STACK_H

#include <cstddef>

struct IMemory
{
    virtual void *malloc(size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual ~IMemory() = default;
};

template <typename T>
class Stack
{
    struct node_t
    {
        T data;
        node_t *next;
        node_t(T _data, node_t *_next) : data{_data}, next{_next} {}
    };

    size_t count{0};
    IMemory &memory;
    node_t *top{nullptr};

public:
    Stack(const Stack &) = delete;
    Stack(IMemory &mem) : memory{mem} {}
    Stack &operator=(const Stack &) = delete;

    Stack(Stack &&that) noexcept : count{that.count}, memory{that.memory}, top{that.top}
    {
        that.count = 0;
        that.top = nullptr;
    }

    Stack &operator=(Stack &&that) noexcept
    {
        if (this != &that)
        {
            clear();

            top = that.top;
            count = that.count;
            memory = that.memory;

            that.count = 0;
            that.top = nullptr;
        }

        return *this;
    }

    bool push(const T &value)
    {
        bool status{false};

        // malloc only allocates memory for a node_t. It does not construct a node_t;
        node_t *node{reinterpret_cast<node_t *>(memory.malloc(sizeof(node_t)))};

        if (node != nullptr)
        {
            // Placement new is used to construct an object in an allocated block of memory.
            (void)new (node) node_t{value, top};

            status = true;
            top = node;
            count++;
        }

        return status;
    }

    bool pop(T &value)
    {
        bool status{false};

        if (top != nullptr)
        {
            status = true;
            value = top->data;

            node_t *temp{top};
            top = top->next;

            temp->~node_t(); // Call the destructor because free does not destruct the object
            memory.free(temp);

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

            temp->~node_t(); // Call the destructor because free does not destruct the object
            memory.free(temp);
        }

        count = 0;
    }

    ~Stack() { clear(); }
};

#endif