/*
    std::lock locks all the specified mutexes in an unspecified order to avoid deadlock
        If any of the locks fail, they are all unlocked (to avoid potential deadlocks) and an exception is thrown.
        Once std::lock returns, all the specified mutexes are successfully locked.
        Look at: https://en.cppreference.com/w/cpp/thread/lock
*/
#include <mutex>
#include <thread>
#include <iostream>

#define AVOID_DEADLOCK_USING_LOCK_GUARD 0
#define AVOID_DEADLOCK_USING_SCOPED_LOCK 0
#define AVOID_DEADLOCK_USING_UNIQUE_LOCK 0

struct Account
{
    int balance;
    std::mutex mtx;
    explicit Account(int _balance) : balance{_balance} {}
};

void transfer(Account &from, Account &to, int amount)
{
    // Avoid deadlock in case of self transfer
    if (&from != &to)
    {
#if AVOID_DEADLOCK_USING_LOCK_GUARD
        std::lock(from.mtx, to.mtx); // Locks both the mutexes without risking deadlock

        // Possible to use the adopt_lock strategy
        std::lock_guard from_lock{from.mtx, std::adopt_lock};
        std::lock_guard to_lock{to.mtx, std::adopt_lock};

        from.balance -= amount;
        to.balance += amount;
#elif AVOID_DEADLOCK_USING_SCOPED_LOCK
#if 0
        std::scoped_lock lock{from.mtx, to.mtx}; // Locks both the mutexes without risking deadlock
#else
        std::lock(from.mtx, to.mtx);                              // Locks both the mutexes without risking deadlock
        std::scoped_lock lock{std::adopt_lock, from.mtx, to.mtx}; // Possible to use the adopt_lock strategy
#endif
        from.balance -= amount;
        to.balance += amount;

#elif AVOID_DEADLOCK_USING_UNIQUE_LOCK
#if 0
        std::lock(from.mtx, to.mtx); // Locks both the mutexes without risking deadlock

        // Possible to use the adopt_lock strategy
        std::unique_lock from_lock{from.mtx, std::adopt_lock};
        std::unique_lock to_lock{to.mtx, std::adopt_lock};

#else
        // Possible to use the defer_lock strategy
        std::unique_lock from_lock{from.mtx, std::defer_lock};
        std::unique_lock to_lock{to.mtx, std::defer_lock};

        std::lock(from_lock, to_lock); // Locks both the mutexes without risking deadlock

#endif
        from.balance -= amount;
        to.balance += amount;

        from_lock.unlock(); // Possible to unlock
        std::this_thread::sleep_for(std::chrono::microseconds(100));
#else
        // What if the threads start to run at the same time?

        std::lock_guard from_lock{from.mtx}; // thrd1 tries to lock myAccount.mtx and thrd2 tries to lock yourAccount.mtx

        std::lock_guard to_lock{to.mtx}; // thrd1 tries to lock yourAccount.mtx and thrd2 tries to lock myAccount.mtx

        from.balance -= amount;
        to.balance += amount;
#endif
    }
}

int main(void)
{
    for (size_t i = 0; i < 500; i++)
    {
        Account myAccount{100};
        Account yourAccount{50};
        std::thread thrd1{transfer, std::ref(myAccount), std::ref(yourAccount), 10};
        std::thread thrd2{transfer, std::ref(yourAccount), std::ref(myAccount), 5};
        thrd1.join();
        thrd2.join();
        std::cout << std::endl
                  << "Run: " << (i + 1) << std::endl;
        std::cout << "myAccount.balance = " << myAccount.balance << std::endl;
        std::cout << "yourAccount.balance = " << yourAccount.balance << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    return 0;
}
