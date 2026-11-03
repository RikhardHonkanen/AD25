#include "portc.h"
#include <core_pins.h>

#define ALL_HIGH (0xFFU)
#define ALL_OUTPUT (0xFFU)
#define PIN_MUX_GPIO (1U << 8)

void portc_init(void)
{
    PORTC_PCR0 = PIN_MUX_GPIO; // Pin 15
    PORTC_PCR1 = PIN_MUX_GPIO; // Pin 22
    PORTC_PCR2 = PIN_MUX_GPIO; // Pin 23
    PORTC_PCR3 = PIN_MUX_GPIO; // Pin 9
    PORTC_PCR4 = PIN_MUX_GPIO; // Pin 10
    PORTC_PCR5 = PIN_MUX_GPIO; // Pin 13, The builtin LED
    PORTC_PCR6 = PIN_MUX_GPIO; // Pin 11
    PORTC_PCR7 = PIN_MUX_GPIO; // Pin 12

    GPIOC_PDDR = ALL_OUTPUT;

    GPIOC_PDOR = ALL_HIGH;
}

volatile uint8_t *portc_address(void)
{
    return (volatile uint8_t *)(&GPIOC_PDOR);
}