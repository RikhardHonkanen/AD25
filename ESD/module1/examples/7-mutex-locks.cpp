/*
    The class lock_guard is a mutex wrapper that provides a Resource Acquisition Is Initialization or RAII mechanism.
        When a lock_guard object is created, it attempts to take ownership of the mutex it is given.
        When control leaves the scope in which the lock_guard object was created, the lock_guard is
        destructed and the mutex is released.
        Look at: https://en.cppreference.com/w/cpp/thread/lock_guard

    The class scoped_lock is a mutex wrapper that provides a Resource Acquisition Is Initialization or RAII mechanism.
        When a scoped_lock object is created, it attempts to take ownership of the mutexes it is given.
        When control leaves the scope in which the scoped_lock object was created, the scoped_lock is
        destructed and the mutexes are released. If several mutexes are given, deadlock avoidance
        algorithm is used as if by std::lock.
        Look at: https://en.cppreference.com/w/cpp/thread/scoped_lock

    The class unique_lock is a general-purpose mutex ownership wrapper allowing deferred locking,
        time-constrained attempts at locking, recursive locking, transfer of lock ownership, and
        use with condition variables. The class unique_lock is movable, but not copyable.
        Look at: https://en.cppreference.com/w/cpp/thread/unique_lock
*/

#include <mutex>
#include <thread>
#include <iostream>
#include <shared_mutex>

static int counter{5};
static std::mutex mtx;

void increment(void)
{
    std::lock_guard<std::mutex> lock(mtx);

    ++counter;

    std::cout << "increment counter: " << counter << std::endl;

    // mtx is automatically released when lock goes out of scope
}

void decrement(void)
{
    std::scoped_lock<std::mutex> lock(mtx);

    --counter;

    std::cout << "decrement counter: " << counter << std::endl;

    // mtx is automatically released when lock goes out of scope
}

void reset(void)
{
    std::unique_lock<std::mutex> lock(mtx);

    counter = 0;

    std::cout << "reset counter: " << counter << std::endl;

    lock.unlock(); // Possible to unlock mtx before
                   // it is automatically released when lock goes out of scope

    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main(void)
{
    std::cout << "counter: " << counter << "; in main()" << std::endl;

    std::thread thrd1(reset);
    std::thread thrd2(decrement);
    std::thread thrd3(increment);

    thrd1.join();
    thrd2.join();
    thrd3.join();

    std::cout << "counter: " << counter << "; in main()" << std::endl;

    return 0;
}