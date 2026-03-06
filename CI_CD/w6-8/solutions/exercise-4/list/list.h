#ifndef LIST_H
#define LIST_H

#include <cstddef>

class List
{
    struct node_t
    {
        int data;
        node_t *next;
    };

    size_t count{0};
    node_t *head{nullptr};

public:
    List() = default;
    List(const List &) = delete;
    List &operator=(const List &) = delete;

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