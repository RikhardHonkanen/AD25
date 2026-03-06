/**
 * @file buffer.h
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Implement and test an uncopyable template circular buffer class according to the requirements below.

    Use a template type (T) and a non-type(N) template parameters.

    A buffer is an array of type T whose length is N

        Ensure that N is at least 4

    The class shall have a function to clear a buffer

    The class shall have a function to read data from a buffer

    The class shall have a function to write data to a buffer

        If the buffer is full then the oldest data shall be overwritten

    The class shall have a function to return the number of data elements stored in a buffer

    The class shall have a function to check if a buffer is full or not
    Overload the insertion operator (<<) for the class to print all the stored elements in a buffer to the terminal

    The class shall be tested using assertion.

 
For G, all the requirements above shall be fulfilled.
For VG, the G requirements shall be fulfilled and if the template type(T) is an arithmetic type then the class shall have a function to return average of the data elements stored in a circular buffer.

    The function shall not remove data elements stored in the buffer.
    The return type of the function shall be double.
 * @version 0.1
 * @date 2026-02-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>
#include <stdexcept>
#include <iostream>

template <typename T, std::size_t N>
class Buffer
{
    static_assert(N >= 4, "Buffer size must be at least 4");

    std::size_t head{0}; 
    std::size_t tail{0}; 
    std::size_t count{0};
    T array[N]{};

public:
    Buffer() = default;

    Buffer(const Buffer &) = delete;            // Uncopyable
    Buffer &operator=(const Buffer &) = delete; // Unassignable

    friend std::ostream& operator<<(std::ostream& os, const Buffer& buffer)
    {
        std::size_t index = buffer.head;

        for (std::size_t i = 0; i < buffer.count; ++i)
        {
            os << "(" << buffer.array[index] << ")\n";
            index = (index + 1) % N;
        }

        return os;
    }

    bool push(const T& item) noexcept
    {
        array[tail] = item;
        tail = (tail + 1) % N;

        if (count == N)
        {
            head = (head + 1) % N;
        }
        else
        {
            ++count;
        }

        return true;
    }

    bool pop(T& item) noexcept
    {
        bool status{false};
        
        if (count != 0) 
        {
            status = true;
            item = array[head];
            head = (head + 1) % N;
            --count;
        }

        return status;
    }

    int capacity(void) noexcept
    {
        return N;
    }

    int available(void) noexcept
    {
        return count;
    }

    bool full(void) noexcept 
    {
        return N == count;
    }

    void clear(void) noexcept
    {
        head = 0;
        tail = 0;
        count = 0;
    }

    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, double>
    average() const
    {
        if (count == 0)
            return 0.0;

        double sum{0.0};
        std::size_t index = head;

        for (std::size_t i = 0; i < count; ++i)
        {
            sum += static_cast<double>(array[index]);
            index = (index + 1) % N;
        }

        return sum / static_cast<double>(count);
    }

    ~Buffer() = default;
};

#endif /* BUFFER_H */