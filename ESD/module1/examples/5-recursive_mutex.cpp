// https://en.cppreference.com/w/cpp/thread/recursive_mutex

#include <mutex>
#include <thread>
#include <iostream>

class X
{
    std::string shared;
    std::recursive_mutex mtx;

public:
    void func1(void)
    {
        mtx.lock();

        shared += "func1";
        std::cout << "in func1 in thread " << std::this_thread::get_id()
                  << " shared variable is now " << shared << std::endl;

        mtx.unlock();
    }

    void func2(void)
    {
        mtx.lock();

        shared += "func2";
        std::cout << "in func2 in thread " << std::this_thread::get_id()
                  << " shared variable is now " << shared << std::endl;
        func1(); // recursive lock becomes useful here
        std::cout << "back in func2, shared variable is " << shared << std::endl;

        mtx.unlock();
    }
};

int main(void)
{
    X x;
    std::thread t1(&X::func1, &x);
    std::thread t2(&X::func2, &x);
    t1.join();
    t2.join();

    return 0;
}
