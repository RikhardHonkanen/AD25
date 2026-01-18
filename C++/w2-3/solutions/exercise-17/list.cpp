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
    int elems_num;
};

list_t *list::create(void)
{
    return new (std::nothrow) list_t{nullptr, 0};
}

int list::available(list_t *lst)
{
    return (lst == nullptr) ? -1 : lst->elems_num;
}

bool list::insert(list_t *lst, int data)
{
    bool status{false};

    if (lst != nullptr)
    {
        // Find position of the new node
        node_t *previous{nullptr};
        node_t *current{lst->head};

        while (current != nullptr)
        {
            if (current->data >= data)
            {
                break;
            }
            previous = current;
            current = current->next;
        }

        if ((current == nullptr) || (current->data != data))
        {
            // Create a node
            node_t *node{new (std::nothrow) node_t{data, nullptr}};

            if (node != nullptr)
            {
                lst->elems_num++;
                status = true;

                node->data = data;
                node->next = nullptr;

                // Add the node to
                if (current == nullptr && previous == nullptr)
                {
                    // the empty list
                    lst->head = node;
                }
                else if (current != nullptr && previous == nullptr)
                {
                    // the begining of the list
                    lst->head = node;
                    node->next = current;
                }
                else if (current == nullptr && previous != nullptr)
                {
                    // the end of the list
                    previous->next = node;
                }
                else
                {
                    // before current node
                    node->next = current;
                    previous->next = node;
                }
            }
        }
    }

    return status;
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

bool list::remove(list_t *list, int data)
{
    bool status{false};

    if (list != nullptr)
    {
        node_t *previous{nullptr};
        node_t *current{list->head};

        while (current != nullptr)
        {
            if (current->data == data)
            {
                if (previous == nullptr)
                {
                    list->head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                status = true;
                delete current;
                list->elems_num--;
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
        if (search(lst, odata))
        {
            if (remove(lst, odata))
            {
                status = insert(lst, ndata);
            }
        }
    }

    return status;
}

void list::destroy(list_t *&lst)
{
    if (lst != nullptr)
    {
        node_t *current{lst->head};

        while (current != nullptr)
        {
            lst->head = current->next;
            delete current;
            current = lst->head;
        }

        delete lst;
        lst = nullptr;
    }
}
