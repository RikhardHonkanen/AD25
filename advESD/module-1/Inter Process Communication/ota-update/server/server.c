/**
 * @file server.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief The server for an example of over the air update.
 *
 * @version 0.1
 * @date 2025-08-15
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
#include <stdio.h>
#include <time.h>

#define SERVER_PORT "12345"
#define SERVER_IP "0.0.0.0"
#define FIRMWARE "../client/.pio/build/esp32-c6-devkitc-1/firmware.bin"

int main(void)
{

    mbedtls_net_context server_ctx, client_ctx;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt srvcert;
    mbedtls_pk_context pkey;
    uint8_t buffer[4096];

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
    for (size_t i = 0; i < 32; i++)
    {
        buffer[i] = rand() % 256;
    }
    assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)buffer, 32));
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    assert(0 == mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT));

    assert(0 == mbedtls_x509_crt_parse_file(&srvcert, "srvrcert.pem"));
    mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);

    assert(0 == mbedtls_pk_parse_keyfile(&pkey, "prvtkey.pem", NULL));
    assert(0 == mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey));

    assert(0 == mbedtls_net_bind(&server_ctx, SERVER_IP, SERVER_PORT, MBEDTLS_NET_PROTO_TCP));

    assert(0 == mbedtls_ssl_setup(&ssl, &conf));

    printf("Waiting on connection ...\n");
    if (0 == mbedtls_net_accept(&server_ctx, &client_ctx, NULL, 0, NULL))
    {
        mbedtls_ssl_set_bio(&ssl, &client_ctx, mbedtls_net_send, NULL, mbedtls_net_recv_timeout);

        if (0 == mbedtls_ssl_handshake(&ssl))
        {
            FILE *file = fopen(FIRMWARE, "rb");

            if (file != NULL)
            {
                if (0 == fseek(file, 0L, SEEK_END))
                {
                    const int size = ftell(file);
                    printf("Firmware Size: %d KB\n", size / 1024);

                    if (size > 0)
                    {
                        if (0 == fseek(file, 0L, SEEK_SET))
                        {
                            size_t len;
                            int counter = 0;
                            int percent = 0;
                            const int five_percent = (int)((float)size / sizeof(buffer) / 20);

                            while (!feof(file))
                            {
                                usleep(5000);

                                len = fread(buffer, sizeof(buffer[0]), sizeof(buffer), file);

                                if (len > 0)
                                {
                                    if (0 >= mbedtls_ssl_write(&ssl, (const uint8_t *)buffer, len))
                                    {
                                        printf("mbedtls_ssl_write failed!\n");
                                        break;
                                    }
                                }
                                else if (ferror(file))
                                {
                                    printf("Failed to read from the file!\n");
                                    break;
                                }

                                counter++;
                                if (counter == five_percent)
                                {
                                    counter = 0;
                                    percent += 5;
                                    printf("Uploaded ... %d%%\n", percent);
                                }
                            }
                        }
                    }
                }

                fclose(file);
            }
            else
            {
                printf("Failed to open firmware.bin ...\n");
            }

            printf("Updating completed!\n");
            assert(0 == mbedtls_ssl_close_notify(&ssl));
        }
        else
        {
            printf("mbedtls_ssl_handshake failed!\n");
        }
    }
    else
    {
        printf("mbedtls_net_accept failed!\n");
    }

    mbedtls_pk_free(&pkey);
    mbedtls_ssl_free(&ssl);
    mbedtls_psa_crypto_free();
    mbedtls_net_free(&client_ctx);
    mbedtls_net_free(&server_ctx);
    mbedtls_entropy_free(&entropy);
    mbedtls_x509_crt_free(&srvcert);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return EXIT_SUCCESS;
}
