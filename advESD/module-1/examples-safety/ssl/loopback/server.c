/**
 * @file server.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of mbedTLS server.
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
#include <mbedtls/x509.h>
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
#include <ctype.h>
#include <time.h>

#define BUFLEN 16
#define SERVER_PORT "12345"
#define SERVER_IP "0.0.0.0"

int main(void)
{
    uint8_t buffer[BUFLEN + 1];

    mbedtls_net_context server_ctx, client_ctx;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt srvcert;
    mbedtls_pk_context pkey;

    mbedtls_ssl_init(&ssl);
    mbedtls_pk_init(&pkey);
    mbedtls_net_init(&client_ctx);
    mbedtls_net_init(&server_ctx);
    mbedtls_ssl_config_init(&conf);
    mbedtls_x509_crt_init(&srvcert);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    assert(PSA_SUCCESS == psa_crypto_init());

    srand(time(NULL));
    for (size_t i = 0; i < BUFLEN; i++)
    {
        buffer[i] = rand() % 256;
    }
    assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)buffer, BUFLEN));
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    assert(0 == mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT));

    assert(0 == mbedtls_x509_crt_parse_file(&srvcert, "srvrcert.pem"));
    mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);

    assert(0 == mbedtls_pk_parse_keyfile(&pkey, "prvtkey.pem", NULL));
    assert(0 == mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey));

    assert(0 == mbedtls_net_bind(&server_ctx, SERVER_IP, SERVER_PORT, MBEDTLS_NET_PROTO_TCP));

    assert(0 == mbedtls_ssl_setup(&ssl, &conf));

    while (1)
    {
        printf("Waiting on a connection ...\n");
        if (0 != mbedtls_net_accept(&server_ctx, &client_ctx, NULL, 0, NULL))
        {
            printf("mbedtls_net_accept failed!\n");
            break;
        }

        mbedtls_ssl_set_bio(&ssl, &client_ctx, mbedtls_net_send, NULL, mbedtls_net_recv_timeout);

        if (0 != mbedtls_ssl_handshake(&ssl))
        {
            printf("mbedtls_ssl_handshake failed!\n");
            break;
        }

        while (1)
        {
            if (0 >= mbedtls_ssl_read(&ssl, (uint8_t *)buffer, BUFLEN))
            {
                printf("mbedtls_ssl_read failed!\n");
                break;
            }

            for (int i = 0; i < BUFLEN; i++)
            {
                buffer[i] = toupper(buffer[i]);
            }

            if (0 >= mbedtls_ssl_write(&ssl, (const uint8_t *)buffer, BUFLEN))
            {
                printf("mbedtls_ssl_write failed!\n");
                break;
            }
        }

        printf("Connection closed ...\n");

        mbedtls_net_free(&client_ctx);

        if (0 != mbedtls_ssl_session_reset(&ssl))
        {
            printf("mbedtls_ssl_session_reset failed!\n");
            break;
        }
    }

    mbedtls_pk_free(&pkey);
    mbedtls_ssl_free(&ssl);
    mbedtls_psa_crypto_free();
    mbedtls_net_free(&server_ctx);
    mbedtls_entropy_free(&entropy);
    mbedtls_x509_crt_free(&srvcert);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return EXIT_FAILURE;
}
