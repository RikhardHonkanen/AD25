/**
 * @file client.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of an mbedtls client.
 *
 * @version 0.1
 * @date 2025-04-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <mbedtls/net_sockets.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ssl.h>
#include <psa/crypto.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define BUFLEN 16
#define SERVER_PORT "12345"
#define SERVER_IP "192.168.4.1"

int main(void)
{
    uint8_t buffer[BUFLEN + 1] = {0};

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_net_context conn_ctx;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    assert(PSA_SUCCESS == psa_crypto_init());
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ssl_config_init(&conf);
    mbedtls_entropy_init(&entropy);
    mbedtls_net_init(&conn_ctx);
    mbedtls_ssl_init(&ssl);

    srand(time(NULL));
    for (size_t i = 0; i < BUFLEN; i++)
    {
        buffer[i] = rand() % 256;
    }
    assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)buffer, BUFLEN));
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    assert(0 == mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT));

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE); // It shall be MBEDTLS_SSL_VERIFY_REQUIRED in a real product

    assert(0 == mbedtls_ssl_setup(&ssl, &conf));

    assert(0 == mbedtls_net_connect(&conn_ctx, SERVER_IP, SERVER_PORT, MBEDTLS_NET_PROTO_TCP));

    mbedtls_ssl_set_bio(&ssl, &conn_ctx, mbedtls_net_send, NULL, mbedtls_net_recv_timeout);

    assert(0 == mbedtls_ssl_handshake(&ssl));

    while (1)
    {
        assert(0 == mbedtls_ctr_drbg_random(&ctr_drbg, buffer, BUFLEN));

        for (int i = 0; i < BUFLEN; i++)
        {
            buffer[i] = 'a' + buffer[i] % 26;
        }
        printf("    Sent: %s\n", buffer);

        if (0 >= mbedtls_ssl_write(&ssl, (const uint8_t *)buffer, BUFLEN))
        {
            printf("mbedtls_ssl_write failed!\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        if (0 >= mbedtls_ssl_read(&ssl, buffer, BUFLEN))
        {
            printf("mbedtls_ssl_read failed!\n");
            break;
        }

        printf("Received: %s\n\n", buffer);

        sleep(1);
    }

    assert(0 == mbedtls_ssl_close_notify(&ssl));

    mbedtls_ssl_free(&ssl);
    mbedtls_psa_crypto_free();
    mbedtls_net_free(&conn_ctx);
    mbedtls_ssl_config_free(&conf);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return EXIT_FAILURE;
}
