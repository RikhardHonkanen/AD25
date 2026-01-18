#include "list.h"
#include <cassert>
#include <iostream>

int main(void)
{
    list_t *mylist = list::create();

    assert(mylist != nullptr);
    assert(0 == list::available(mylist));
    assert(!list::remove(mylist, 0));

    assert(list::insert(mylist, 30));
    assert(1 == list::available(mylist));

    assert(list::insert(mylist, 20));
    assert(2 == list::available(mylist));

    assert(list::insert(mylist, 10));
    assert(3 == list::available(mylist));

    assert(list::search(mylist, 10));
    assert(list::search(mylist, 20));
    assert(list::search(mylist, 30));
    assert(!list::search(mylist, 15));

    assert(list::edit(mylist, 20, 15));
    assert(!list::search(mylist, 20));
    assert(list::search(mylist, 15));

    assert(list::remove(mylist, 10));
    assert(2 == list::available(mylist));

    assert(list::remove(mylist, 15));
    assert(1 == list::available(mylist));

    list::destroy(mylist);
    assert(mylist == nullptr);

    std::cout << "All the tests passed!" << std::endl;

    return 0;
}