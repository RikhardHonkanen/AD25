#ifndef RNGA_H
#define RNGA_H

#include <kinetis.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    void rnga_init(void);
    void rnga_start(void);
    void rnga_seed(uint32_t seed);
    uint32_t rnga_generate(uint32_t min, uint32_t max);
    void rnga_end(void);
#ifdef __cplusplus
}
#endif

#endif
