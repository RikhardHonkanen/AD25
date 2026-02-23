#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <type_traits>

struct IMemory
{
    virtual void *malloc(size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual ~IMemory() = default;
};

template <typename T>
class Queue
{
    struct node_t
    {
        T data;
        node_t *next;
    };

    IMemory &memory;
    size_t count{0};
    node_t *head{nullptr};
    node_t *tail{nullptr};

    node_t *create_node(const T &data)
    {
        // malloc only allocates memory for a node_t. It does not construct a node_t;
        node_t *node = reinterpret_cast<node_t *>(memory.malloc(sizeof(node_t)));

        if (node != nullptr)
        {
            // The placement new is used to construct an object in an allocated block of memory.
            (void)new (node) node_t{data, nullptr};
        }

        return node;
    }

    void delete_node(node_t *node)
    {
        // The node destructor shall be called
        node->~node_t();

        memory.free(node);
    }

public:
    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;

    Queue(IMemory &_memory) : memory{_memory} {}

    Queue(Queue &&that) noexcept;

    Queue &operator=(Queue &&that) noexcept;

    bool enqueue(const T &item);

    bool dequeue(T &item);

    size_t size(void) { return count; }

    void clear(void);

    ~Queue() { clear(); }
};

template <typename T>
Queue<T>::Queue(Queue<T> &&that) noexcept : memory{that.memory}, count{that.count}, head{that.head}, tail{that.tail}
{
    that.count = 0;
    that.head = nullptr;
    that.tail = nullptr;
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue<T> &&that) noexcept
{
    if (this != &that)
    {
        clear();

        memory = that.memory;
        count = that.count;
        head = that.head;
        tail = that.tail;

        that.count = 0;
        that.head = nullptr;
        that.tail = nullptr;
    }

    return *this;
}

template <typename T>
bool Queue<T>::enqueue(const T &item)
{
    bool status{false};

    node_t *node{create_node(item)};

    if (node != nullptr)
    {
        if (head == nullptr)
        {
            head = node;
            tail = head;
        }
        else
        {
            tail->next = node;
            tail = node;
        }

        status = true;
        count++;
    }

    return status;
}

template <typename T>
bool Queue<T>::dequeue(T &item)
{
    bool status{false};

    if (head != nullptr)
    {
        item = head->data;

        node_t *temp{head};
        head = head->next;

        delete_node(temp);

        if (head == nullptr)
        {
            tail = head;
        }

        status = true;
        count--;
    }

    return status;
}

template <typename T>
void Queue<T>::clear(void)
{
    while (head != nullptr)
    {
        tail = head;
        head = head->next;
        delete_node(tail);
    }

    count = 0;
    tail = head;
}

#endif
