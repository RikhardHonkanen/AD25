#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>

struct buffer_t;

namespace buffer
{
    buffer_t *create(size_t size);

    bool insert(buffer_t *buf, size_t start, size_t length, int value);

    bool insert(buffer_t *buf, size_t start, size_t length, float value);

    bool extract(buffer_t *buf, size_t start, size_t length, int &data);

    bool extract(buffer_t *buf, size_t start, size_t length, float &data);

    void destroy(buffer_t *&buf);

    void clear(buffer_t *buf);
}

#endif