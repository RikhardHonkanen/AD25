#ifndef LIST_H
#define LIST_H

#include <cstddef>

struct IMemory
{
    virtual void *malloc(size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual ~IMemory() = default;
};

class List
{
    struct node_t
    {
        int data;
        node_t *next{nullptr};
    };

    size_t count{0};
    node_t *head{nullptr};
    IMemory *memory{nullptr};

public:
    List(const List &) = delete;
    List &operator=(const List &) = delete;

    List(IMemory *imemory) : memory{imemory} {}

    size_t size(void) { return count; }

    List(List &&that) noexcept;

    List &operator=(List &&that) noexcept;

    bool edit(int cvalue, int nvalue);

    size_t search(int value);

    bool remove(int value);

    bool insert(int item);

    void clear(void);

    ~List() { clear(); }
};

#endif