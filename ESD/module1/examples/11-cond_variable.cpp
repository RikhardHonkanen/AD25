// https://en.cppreference.com/w/cpp/thread/condition_variable

#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <condition_variable>

static std::mutex mtx;
static std::string data;
static bool ready = false;
static bool processed = false;
static std::condition_variable cv;

void worker_thread(void)
{
    // Wait until main() sends data
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []
            { return ready; });

    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data" << std::endl;
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed" << std::endl;

    // The notifying thread does not need to hold the lock on the same mutex as the one held by the waiting thread(s);
    lock.unlock(); // Manual unlocking is done before notifying, to avoid waking up.
    cv.notify_one();
}

int main(void)
{
    std::thread worker(worker_thread);

    data = "Example data";

    // send data to the worker thread
    {
        std::lock_guard lock(mtx);
        ready = true;
        std::cout << "main() signals data ready for processing" << std::endl;
    }

    cv.notify_one();

    // wait for the worker
    {
        std::unique_lock lock(mtx);
        while (!processed)
        {
            cv.wait(lock);
        }
    }

    std::cout << "Back in main(), data = " << data << std::endl;

    worker.join();

    return 0;
}