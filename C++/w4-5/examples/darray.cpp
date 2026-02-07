/**
 * @file darray.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a only-movable dynamic size array class template.
 *
 * @version 0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <cassert>
#include <iostream>
#include <initializer_list>

template <typename T>
class DArray
{
    T *ptr;
    size_t size;

public:
    DArray(const DArray &) = delete;
    DArray &operator=(const DArray &) = delete;
    DArray(size_t _size) : ptr{new T[_size]{}}, size{_size} {}
    DArray(const std::initializer_list<T> &list) : ptr{new T[list.size()]{}}, size{list.size()}
    {
        auto temp{ptr};
        auto it{list.begin()};
        while (it < list.end())
        {
            *temp = *it;
            temp++;
            it++;
        }
    }

    void resize(size_t _size);

    DArray(DArray &&that) noexcept : size{that.size}, ptr{that.ptr}
    {
        that.ptr = nullptr;
        that.size = 0;
    }

    DArray &operator=(DArray &&that) noexcept
    {
        if (this != &that)
        {
            delete[] ptr;

            ptr = that.ptr;
            size = that.size;

            that.ptr = nullptr;
            that.size = 0;
        }

        return *this;
    }

    size_t getSize(void) { return size; }

    T &operator[](size_t index)
    {
        if (index >= size)
        {
            throw std::range_error{"Invalid index"};
        }

        return ptr[index];
    }

    T operator[](size_t index) const
    {
        if (index >= size)
        {
            throw std::range_error{"Invalid index"};
        }

        return ptr[index];
    }

    friend std::ostream &operator<<(std::ostream &os, const DArray &array)
    {
        os << "[";
        for (size_t i = 0; i < array.size; i++)
        {
            if (i < array.size - 1)
            {
                os << array.ptr[i] << ", ";
            }
            else
            {
                os << array.ptr[i];
            }
        }
        os << "]" << std::endl;

        return os;
    }

    ~DArray() { delete[] ptr; }
};

template <typename T>
void DArray<T>::resize(size_t _size)
{
    if (_size > size)
    {
        T *temp{new T[_size]{}};

        for (size_t i = 0; i < size; i++)
        {
            temp[i] = ptr[i];
        }

        delete[] ptr;
        size = _size;
        ptr = temp;
    }
    else if (_size < size)
    {
        T *temp{new T[_size]};

        for (size_t i = 0; i < _size; i++)
        {
            temp[i] = ptr[i];
        }

        delete[] ptr;
        size = _size;
        ptr = temp;
    }
    else
    {
        ;
    }
}

int main(void)
{
    DArray<int> arr(2);
    assert(2 == arr.getSize());
    std::cout << "arr = " << arr;
    arr[0] = 10;
    arr[1] = 20;
    std::cout << "arr = " << arr << std::endl;

    DArray<int> array{1, 2};
    assert(2 == array.getSize());
    std::cout << "array = " << array;

    array.resize(4);
    assert(4 == array.getSize());

    array[2] = 3;
    array[3] = 4;
    std::cout << "array = " << array;

    DArray<int> temp{std::move(array)};

    std::cout << std::endl
              << "---- array moved to temp ----" << std::endl;

    assert(0 == array.getSize());
    std::cout << "array = " << array;

    assert(4 == temp.getSize());
    std::cout << " temp = " << temp;

    std::cout << std::endl
              << "---- resize temp ----" << std::endl;

    temp.resize(2);
    assert(2 == temp.getSize());
    std::cout << " temp = " << temp;

    std::cout << std::endl
              << "---- temp moved to array ----" << std::endl;

    array = std::move(temp);
    assert(2 == array.getSize());
    std::cout << "array = " << array << std::endl;

    return 0;
}
