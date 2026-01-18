#include "list.h"
#include <cassert>
#include <iostream>

int main(void)
{
    assert(list::insert(10) && "Failed to insert 10 into the list");
    assert(list::insert(20) && "Failed to insert 20 into the list");
    assert(list::insert(30) && "Failed to insert 30 into the list");
    assert(3 == list::available());
    std::cout << "==> Test Insert: Passed!\n";

    assert(list::search(10));
    assert(list::search(20));
    assert(list::search(30));
    assert(!list::search(300));
    std::cout << "==> Test Search: Passed!\n";

    assert(list::remove(20));
    assert(!list::remove(300));
    assert(2 == list::available());
    assert(list::search(10));
    assert(!list::search(20));
    assert(list::search(30));
    std::cout << "==> Test Remove: Passed!\n";

    assert(list::edit(10, 100));
    assert(list::edit(30, 300));
    assert(2 == list::available());
    assert(!list::edit(20, 200));
    std::cout << "==> Test Edit: Passed!\n";

    list::clear();
    assert(0 == list::available());
    std::cout << "==> Test Clear: Passed!\n";

    std::cout << "******* All the tests passed *********\n";

    return 0;
}
