/**
 * @file random.h
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An module to use the ELS RNG and mbedtls_ctr_drbg_random to generate random data.
 *        Ensure that you add the following components to your project:
 *            1) Software Components -> els_pkc (select main, platform -> mcxn, static_lib_mcxn)
 *            2) Middleware -> Security -> mbedTLSv3 library
 *        Select the project and then click on Edit project setting -> C/C++ Build -> Settings -> Optimization
 *        Chenge the optimization level to O3
 *
 * @version 0.1
 * @date 2025-10-19
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef RANDOM_H
#define RANDOM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool random_init(void);

bool random_get(uint8_t *buffer, size_t size);

#endif /* RANDOM_H */
