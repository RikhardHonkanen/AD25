#include <new>
#include <cstring>
#include <climits>
#include <cstdint>
#include "buffer.h"

struct buffer_t
{
    size_t size;
    uint8_t *array;
};

constexpr float PRECISION{0.01f};

buffer_t *buffer::create(size_t size)
{
    buffer_t *ptr{nullptr};

    if (size > 0)
    {
        ptr = new (std::nothrow) buffer_t{size, nullptr};

        if (ptr != nullptr)
        {
            ptr->array = new (std::nothrow) uint8_t[size]{};

            if (ptr->array == nullptr)
            {
                delete ptr;
                ptr = nullptr;
            }
        }
    }

    return ptr;
}

bool buffer::insert(buffer_t *buf, size_t start, size_t length, int value)
{
    bool status{false};

    if ((buf != nullptr) && (length > 0) && (length <= (sizeof(int) * CHAR_BIT)))
    {
        if (start + length < (buf->size * CHAR_BIT))
        {
            status = true;
            int pos = start % CHAR_BIT;
            int index = start / CHAR_BIT;

            for (size_t i = 0; i < length; i++)
            {
                uint8_t bit = (uint8_t)((value >> i) & 1);

                if (bit == 0)
                {
                    buf->array[index] &= ~(1 << pos);
                }
                else
                {
                    buf->array[index] |= (1 << pos);
                }

                pos++;
                if (pos == CHAR_BIT)
                {
                    pos = 0;
                    index++;
                }
            }
        }
    }

    return status;
}

bool buffer::insert(buffer_t *buf, size_t start, size_t length, float value)
{
    return insert(buf, start, length, static_cast<int>(value / PRECISION));
}

bool buffer::extract(buffer_t *buf, size_t start, size_t length, int &data)
{
    bool status{false};

    if ((buf != nullptr) && (length > 0) && (length <= (sizeof(int) * CHAR_BIT)))
    {
        if (start + length < (buf->size * CHAR_BIT))
        {
            data = 0;
            status = true;

            int pos = start % CHAR_BIT;
            int index = start / CHAR_BIT;

            for (size_t i = 0; i < length; i++)
            {
                uint8_t bit = (uint8_t)((buf->array[index] >> pos) & 1);

                if (bit == 1)
                {
                    data |= (1 << i);
                }

                pos++;
                if (pos == CHAR_BIT)
                {
                    pos = 0;
                    index++;
                }
            }

            if ((data & (1 << (length - 1))) != 0)
            {
                data |= (~0 << length);
            }
        }
    }

    return status;
}

bool buffer::extract(buffer_t *buf, size_t start, size_t length, float &data)
{
    int temp{0};
    bool status{false};

    if (extract(buf, start, length, temp))
    {
        status = true;
        data = temp * PRECISION;
    }

    return status;
}

void buffer::clear(buffer_t *buf)
{
    if ((buf != nullptr) && (buf->array != nullptr))
    {
        std::memset(buf->array, 0, buf->size);
    }
}

void buffer::destroy(buffer_t *&buf)
{
    if (buf != nullptr)
    {
        delete[] buf->array;
        delete buf;
        buf = nullptr;
    }
}