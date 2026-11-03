#ifndef PORTC_H
#define PORTC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void portc_init(void);

    volatile uint8_t *portc_address(void);

#ifdef __cplusplus
}
#endif

#endif