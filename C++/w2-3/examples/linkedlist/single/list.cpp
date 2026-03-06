#include <new>
#include "list.h"

struct node_t
{
    int data;
    node_t *next;
};

static size_t elem_num{0};
static node_t *head{nullptr};

bool list::insert(int data)
{
    bool status{false};

    node_t *node{new (std::nothrow) node_t{data, nullptr}};

    if (node != nullptr)
    {
        if (head == nullptr)
        {
            head = node;
        }
        else
        {
            node_t *cursor = head;
            while (cursor->next != nullptr)
            {
                cursor = cursor->next;
            }
            cursor->next = node;
        }

        status = true;
        elem_num++;
    }

    return status;
}

size_t list::available(void)
{
    return elem_num;
}

bool list::search(int data)
{
    bool found{false};

    node_t *cursor = head;

    while (cursor != nullptr)
    {
        if (cursor->data == data)
        {
            found = true;
            break;
        }

        cursor = cursor->next;
    }

    return found;
}

bool list::remove(int data)
{
    bool status{false};
    node_t *current{head};
    node_t *previous{nullptr};

    while (current != nullptr)
    {
        if (current->data == data)
        {
            if (previous == nullptr)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            status = true;
            elem_num--;
            break;
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }

    return status;
}

bool list::edit(int odata, int ndata)
{
    bool status{false};

    node_t *cursor = head;

    while (cursor != nullptr)
    {
        if (cursor->data == odata)
        {
            cursor->data = ndata;
            status = true;
            break;
        }

        cursor = cursor->next;
    }

    return status;
}

void list::clear(void)
{
    node_t *current;

    while (head != nullptr)
    {
        current = head;
        head = head->next;
        delete current;
    }

    elem_num = 0;
}
