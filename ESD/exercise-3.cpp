/**
 * @file exercise-3.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief Implement a thread-safe uncopyable template stack class using an array.
 *        When a stack is empty, if a thread tries to pop it shall be suspended.
 *        When a stack is full, if a thread tries to push it shall be suspended.
 *
 *        A piece of code is thread-safe if it functions correctly during simultaneous execution by multiple threads.
 *        In particular, it must satisfy the need for multiple threads to access the same shared data and the shared
 *        data shall be accessed by only one thread at any given time.
 *
 * @version 0.1
 * @date 2024-04-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <ctime>
#include <mutex>
#include <array>
#include <thread>
#include <iostream>
#include <condition_variable>

template <typename T, size_t SIZE>
class Stack
{
    int top{-1};
    T array[SIZE];
    std::mutex mtx;
    std::condition_variable cv;

    bool isempty(void) { return (top == -1); }
    bool isfull(void) { return ((top + 1) == SIZE); }

public:
    using Type = T;
    constexpr static size_t Size{SIZE};

    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;
    Stack() { static_assert(SIZE > 3, "SIZE shall be at least 4"); }

    void push(const T &item)
    {
        std::unique_lock lock{mtx};
        cv.wait(lock, [this]
                { return !this->isfull(); });
        top++;
        array[top] = item;

        lock.unlock();
        cv.notify_all();
    }

    T pop(void)
    {
        std::unique_lock lock{mtx};
        cv.wait(lock, [this]
                { return !this->isempty(); });

        T item{array[top]};
        top--;

        lock.unlock();
        cv.notify_all();

        return item;
    }
};

template <typename T, size_t SIZE>
static void consumer(Stack<T, SIZE> &stack)
{
    T value;
    static std::mutex omtx;

    while (1)
    {
        value = stack.pop();

        omtx.lock();
        std::cout << "[" << std::this_thread::get_id() << "] Consumed: " << value << std::endl;
        omtx.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 501)));
    }
}

int main(void)
{
    int value{0};
    constexpr int CONSUMERS{5};
    Stack<decltype(value), 8> stack;
    std::array<std::thread, CONSUMERS> cthreads;

    std::srand(std::time(nullptr));

    for (int i = 0; i < CONSUMERS; i++)
    {
        cthreads[i] = std::thread{consumer<decltype(stack)::Type, decltype(stack)::Size>, std::ref(stack)};
        cthreads[i].detach();
    }

    while (1)
    {
        value++;

        stack.push(value);

        std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 501)));
    }

    return 0;
}
