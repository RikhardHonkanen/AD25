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

template <typename T>
class Buffer
{
    int top{-1};
    int size{0};
    int items{0};
    T *array{nullptr};
    static constexpr int SIZE_MIN{4};

public:
    Buffer(const Buffer &) = delete;            // Uncopyable

    friend std::ostream &operator<<(std::ostream &out, const Buffer &buffer)
    {
        for (size_t i{0}; i < buffer.items; i++) {
            out << "(" << buffer.array[i] << ")" << std::endl;
        }
        return out;
    }

    Buffer(int _size) : size{_size}
    {
        if (size < SIZE_MIN)
        {
            throw std::invalid_argument{"Invalid size"};
        }

        array = new T[size]{};
    }

    bool push(const T &item) noexcept
    {
        bool status{false};

        if (top < (size - 1))
        {
            top++;
            status = true;
            array[top] = item;
        }
        else {
            top = 0;
            status = true;
            array[top] = item;
        }
        if (items < size) {
            items++;
        }

        return status;
    }

    bool pop(T &item) noexcept
    {
        bool status{false};

        if (items > 0)
        {
            item = array[top];
            status = true;
            if (top == 0) {
                top = size;
            }
            items--;
            top--;
        }

        return status;
    }

    int capacity(void) noexcept
    {
        return size;
    }

    int available(void) noexcept
    {
        return items;
    }

    bool full(void) noexcept 
    {
        return size == items;
    }

    void clear(void) noexcept
    {
        top = -1;
        items = 0;
    }

    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, double> average(void)
    {
        double sum{0};
        int current = top;

        for (size_t i{0}; i < items; i++) 
        {
            sum += array[current];
            if (current == 0) 
            {
                current = size - 1;
            }
            else
            {
                current--;
            }
        }

        return (items == 0) ? 0 : (sum / items);
    }

    ~Buffer() { delete array; }
};

#endif /* BUFFER_H */