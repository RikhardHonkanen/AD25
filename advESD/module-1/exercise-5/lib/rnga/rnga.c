#include "rnga.h"
#include <limits.h>
#include <stdlib.h>

// RNG_CR			(*(volatile uint32_t *)0x40029000) // RNGA Control Register
// RNG_SR			(*(volatile uint32_t *)0x40029004) // RNGA Status Register
// RNG_ER			(*(volatile uint32_t *)0x40029008) // RNGA Entropy Register
// RNG_OR			(*(volatile uint32_t *)0x4002900C) // RNGA Output Register
// SIM_SCGC6_RNGA	((uint32_t)0x00000200)		// RNGA Clock on APIS0 (base
// addr 40029000)

void rnga_init(void)
{
    SIM_SCGC6 |= SIM_SCGC6_RNGA; // Enable the RNGA clock
    RNG_CR = 0;                  // Reset CR
}
void rnga_start(void)
{
    RNG_CR |= (1U << 0); // Set the GO bit
}
void rnga_seed(uint32_t seed)
{
    RNG_ER = seed; // Provide rnga with entropy
}
uint32_t rnga_generate(uint32_t min, uint32_t max)
{
    while ((RNG_SR & (0xFFU << 8)) == 0) // 0xFFU << 8, mask for bits 8-15 in SR register
    {
        // Wait for random data, available when OREG_LVL != 0
    }
    return min + (RNG_OR % (max - min + 1)); // Return the 32-bit output register
}
void rnga_end(void)
{
    SIM_SCGC6 &= ~SIM_SCGC6_RNGA; // Disable the RNGA clock
}