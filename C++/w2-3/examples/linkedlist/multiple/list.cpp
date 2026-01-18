#include <new>
#include "list.h"

struct node_t
{
    int data;
    node_t *next;
};

struct list_t
{
    node_t *head;
    int elem_num;
};

list_t *list::create(void)
{
    return new (std::nothrow) list_t{nullptr, 0};
}

bool list::insert(list_t *lst, int data)
{
    bool status{false};

    if (lst != nullptr)
    {
        node_t *node{new (std::nothrow) node_t{data, nullptr}};

        if (node != nullptr)
        {
            if (lst->head == nullptr)
            {
                lst->head = node;
            }
            else
            {
                node_t *cursor = lst->head;
                while (cursor->next != nullptr)
                {
                    cursor = cursor->next;
                }
                cursor->next = node;
            }

            lst->elem_num++;
            status = true;
        }
    }

    return status;
}

int list::available(list_t *lst)
{
    return (lst != nullptr) ? lst->elem_num : -1;
}

bool list::search(list_t *lst, int data)
{
    bool status{false};

    if (lst != nullptr)
    {
        node_t *cursor{lst->head};

        while (cursor != nullptr)
        {
            if (cursor->data == data)
            {
                status = true;
                break;
            }

            cursor = cursor->next;
        }
    }

    return status;
}

bool list::remove(list_t *lst, int data)
{
    bool status{false};

    if (lst != nullptr)
    {
        node_t *previous{nullptr};
        node_t *current{lst->head};

        while (current != nullptr)
        {
            if (current->data == data)
            {
                if (previous == nullptr)
                {
                    lst->head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                delete current;
                status = true;
                lst->elem_num--;
                break;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
    }

    return status;
}

bool list::edit(list_t *lst, int odata, int ndata)
{
    bool status{false};

    if (lst != nullptr)
    {
        node_t *cursor{lst->head};

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
    }

    return status;
}

void list::clear(list_t *lst)
{
    if (lst != nullptr)
    {
        node_t *current;

        while (lst->head != nullptr)
        {
            current = lst->head;
            lst->head = lst->head->next;
            delete current;
        }

        lst->elem_num = 0;
    }
}

void list::destroy(list_t *&lst)
{
    if (lst != nullptr)
    {
        node_t *current;

        while (lst->head != nullptr)
        {
            current = lst->head;
            lst->head = lst->head->next;
            delete current;
        }

        delete lst;
        lst = nullptr;
    }
}
