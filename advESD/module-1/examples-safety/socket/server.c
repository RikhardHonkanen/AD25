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
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/bignum.h>
#include <psa/crypto.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define STR(x) #x
#define STRINGIFY(x) STR(x)

#ifndef RSA_BITS
#define RSA_BITS 2048
#endif

#define RSA_SIZE (RSA_BITS / CHAR_BIT)
#define OAEP_SALT "{R+5@Vq=bP:LtK0[Ud"
#define AES_BLOCK_SIZE 16
#define AES_IV_SIZE 16
#define HASH_SIZE 32
#define HMAC_SIZE 32
#define AES_SIZE 32

#define PORT 12345

static int connfd;
static uint8_t SECRET[HASH_SIZE];
static uint64_t session_id, seq_number;
static mbedtls_ctr_drbg_context ctr_drbg;
static psa_key_handle_t server_private_key, client_key, server_key, hmac_key;

static void session_reset(void)
{
    psa_destroy_key(client_key);
    psa_destroy_key(server_key);
    psa_destroy_key(hmac_key);
    session_id = 0;
    seq_number = 0;
}

static bool hexstr_to_bin(const char *hex, uint8_t *bin)
{
    bool status = false;

    if (2 * HASH_SIZE == strlen(hex))
    {
        status = true;

        for (size_t i = 0; i < HASH_SIZE; i++)
        {
            char high = hex[2 * i];
            char low = hex[2 * i + 1];

            if (!isxdigit(high) || !isxdigit(low))
            {
                status = false;
                break;
            }

            bin[i] = (uint8_t)((uint8_t)(isdigit(high) ? (high - '0') : (tolower(high) - 'a' + 10)) << 4) |
                     (uint8_t)(isdigit(low) ? (low - '0') : (tolower(low) - 'a' + 10));
        }
    }

    return status;
}

static psa_status_t psa_import_key_from_file(const char *fname, psa_key_id_t *kid, psa_key_type_t ktype, size_t ksize, psa_key_usage_t uflags, psa_algorithm_t algrthm)
{
    FILE *file = fopen(fname, "rb");
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;

    if (file != NULL)
    {
        uint8_t buffer[1 << 16];

        (void)fseek(file, 0, SEEK_END);
        const long size = ftell(file);
        (void)fseek(file, 0, SEEK_SET);

        if ((size > 0) && ((size_t)size < sizeof(buffer)))
        {
            status = PSA_ERROR_GENERIC_ERROR;
            if ((size_t)size == fread(buffer, sizeof(buffer[0]), (size_t)size, file))
            {
                (void)fclose(file);
                buffer[size] = '\0';

                psa_key_attributes_t attrs = PSA_KEY_ATTRIBUTES_INIT;

                psa_set_key_type(&attrs, ktype);
                psa_set_key_bits(&attrs, ksize);
                psa_set_key_usage_flags(&attrs, uflags);
                psa_set_key_algorithm(&attrs, algrthm);

                status = psa_import_key(&attrs, buffer, size + 1, kid);

                psa_reset_key_attributes(&attrs);
                memset(buffer, 0, sizeof(buffer));
            }
            else
            {
                (void)fclose(file);
            }
        }
        else
        {
            (void)fclose(file);
        }
    }

    return status;
}

static bool establish_session(int cfd)
{
    size_t len, length;
    bool status = false;
    uint8_t buffer[2 * RSA_SIZE];
    uint8_t master_secret[HMAC_SIZE];
    uint8_t transcript[4 * RSA_SIZE] = {0};

    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    mbedtls_mpi P, G, Q, SPK, CPK, H;
    psa_key_handle_t master_key;
    mbedtls_mpi_init(&SPK); // Server public key
    mbedtls_mpi_init(&CPK); // Client public key
    mbedtls_mpi_init(&P);   // MODP as a prime
    mbedtls_mpi_init(&G);   // The generator
    mbedtls_mpi_init(&Q);   // Private key
    mbedtls_mpi_init(&H);   // Shared secret

    if (2 * RSA_SIZE != read(cfd, transcript, 2 * RSA_SIZE))
    {
        goto end;
    }

    if (PSA_SUCCESS != psa_asymmetric_decrypt(server_private_key, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256), transcript, RSA_SIZE,
                                              (const uint8_t *)OAEP_SALT, sizeof(OAEP_SALT) - 1, buffer, RSA_SIZE, &len))
    {
        goto end;
    }
    length = len;
    if (PSA_SUCCESS != psa_asymmetric_decrypt(server_private_key, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256), transcript + RSA_SIZE, RSA_SIZE,
                                              (const uint8_t *)OAEP_SALT, sizeof(OAEP_SALT) - 1, buffer + length, RSA_SIZE, &len))
    {
        goto end;
    }
    length += len;

    if ((length != RSA_SIZE) || (0 != mbedtls_mpi_read_binary_le(&P, buffer, RSA_SIZE)))
    {
        goto end;
    }

    // Generator = 2
    if (0 != mbedtls_mpi_lset(&G, 2))
    {
        goto end;
    }

    // Generate private Q, compute public SPK = G^Q mod P
    if ((0 != mbedtls_mpi_fill_random(&Q, RSA_SIZE, mbedtls_ctr_drbg_random, &ctr_drbg)) ||
        (0 != mbedtls_mpi_exp_mod(&SPK, &G, &Q, &P, NULL)))
    {
        goto end;
    }

    if (0 != mbedtls_mpi_write_binary_le(&SPK, transcript + 2 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    if (RSA_SIZE != (size_t)write(cfd, transcript + 2 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    if (RSA_SIZE != (size_t)read(cfd, transcript + 3 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    if (0 != mbedtls_mpi_read_binary_le(&CPK, transcript + 3 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    // Compute the shared secret. H = CPK ^ Q mod P
    if (0 != mbedtls_mpi_exp_mod(&H, &CPK, &Q, &P, NULL))
    {
        goto end;
    }

    // Write the shared secret into buffer
    if (0 != mbedtls_mpi_write_binary_le(&H, buffer, RSA_SIZE))
    {
        goto end;
    }

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attr, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_bits(&attr, HMAC_SIZE * CHAR_BIT);
    psa_set_key_type(&attr, PSA_KEY_TYPE_HMAC);
    if (PSA_SUCCESS != psa_import_key(&attr, SECRET, HMAC_SIZE, &master_key))
    {
        goto end;
    }

    // Using SECRET compute mac of the shared secret as the master secret
    if (PSA_SUCCESS != psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, RSA_SIZE, master_secret, HMAC_SIZE, &length))
    {
        goto end;
    }

    if (PSA_SUCCESS != psa_import_key(&attr, master_secret, HMAC_SIZE, &master_key))
    {
        goto end;
    }

    // Using the master secret generate the authentication keys
    uint8_t secret_keys[2][HMAC_SIZE];
    if ((PSA_SUCCESS != psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), (const uint8_t *)"client authentication key",
                                        strlen("client authentication key"), secret_keys[0], HMAC_SIZE, &length)) ||
        (PSA_SUCCESS != psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), (const uint8_t *)"server authentication key",
                                        strlen("server authentication key"), secret_keys[1], HMAC_SIZE, &length)))
    {
        goto end;
    }

    // Hash the transcript
    if (PSA_SUCCESS != psa_hash_compute(PSA_ALG_SHA_256, transcript, sizeof(transcript), buffer, HASH_SIZE, &len))
    {
        goto end;
    }

    // Import authentication key
    if ((PSA_SUCCESS != psa_import_key(&attr, secret_keys[0], HMAC_SIZE, &client_key)) ||
        (PSA_SUCCESS != psa_import_key(&attr, secret_keys[1], HMAC_SIZE, &server_key)))
    {
        goto end;
    }

    if (HMAC_SIZE != (size_t)read(cfd, transcript, HMAC_SIZE))
    {
        goto end;
    }

    if (PSA_SUCCESS != psa_mac_verify(client_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, HASH_SIZE, transcript, HMAC_SIZE))
    {
        goto end;
    }

    // Compute the hashed transcript mac using the server authentication key
    if (PSA_SUCCESS != psa_mac_compute(server_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, HASH_SIZE, transcript, HMAC_SIZE, &length))
    {
        goto end;
    }

    if (HMAC_SIZE != (size_t)write(cfd, transcript, HMAC_SIZE))
    {
        goto end;
    }

    // Using the master secret generate the hmac key and th AES keys
    if ((PSA_SUCCESS != psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), (const uint8_t *)"client key",
                                        strlen("client key"), secret_keys[0], HMAC_SIZE, &length)) ||
        (PSA_SUCCESS != psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), (const uint8_t *)"server key",
                                        strlen("server key"), secret_keys[1], HMAC_SIZE, &length)) ||
        (PSA_SUCCESS != psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), (const uint8_t *)"hmac key",
                                        strlen("hmac key"), buffer, HMAC_SIZE, &length)))
    {
        goto end;
    }

    if (PSA_SUCCESS != psa_import_key(&attr, buffer, HMAC_SIZE, &hmac_key))
    {
        goto end;
    }

    psa_reset_key_attributes(&attr);
    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attr, PSA_ALG_CBC_NO_PADDING);
    psa_set_key_type(&attr, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attr, AES_SIZE * CHAR_BIT);
    if (PSA_SUCCESS != psa_import_key(&attr, secret_keys[0], AES_SIZE, &client_key))
    {
        goto end;
    }

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_ENCRYPT);
    if (PSA_SUCCESS != psa_import_key(&attr, secret_keys[1], AES_SIZE, &server_key))
    {
        goto end;
    }

    if (PSA_SUCCESS == psa_mac_compute(master_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), (const uint8_t *)"session", strlen("session"), buffer, HMAC_SIZE, &length))
    {
        status = true;
        memcpy(&session_id, buffer, sizeof(session_id));
        memcpy(&seq_number, buffer + sizeof(session_id), sizeof(seq_number));
    }

end:
    mbedtls_mpi_free(&H);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&G);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&SPK);
    mbedtls_mpi_free(&CPK);
    psa_destroy_key(master_key);
    if (!status)
    {
        session_reset();
    }
    return status;
}

static bool request(uint8_t *req)
{
    uint64_t counter;
    size_t len, length;
    bool status = false;
    uint8_t buffer[sizeof(session_id) + sizeof(seq_number) + AES_IV_SIZE + AES_BLOCK_SIZE + HMAC_SIZE];

    if (sizeof(buffer) == read(connfd, buffer, sizeof(buffer)))
    {
        if (PSA_SUCCESS == psa_mac_verify(hmac_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, sizeof(buffer) - HMAC_SIZE, buffer + sizeof(buffer) - HMAC_SIZE, HMAC_SIZE))
        {
            memcpy(&counter, buffer + sizeof(session_id), sizeof(counter));
            if ((0 == memcmp(&session_id, buffer, sizeof(session_id))) && (counter > seq_number))
            {
                seq_number = counter;

                len = sizeof(session_id) + sizeof(seq_number);
                if (PSA_SUCCESS == psa_cipher_decrypt(client_key, PSA_ALG_CBC_NO_PADDING, buffer + len, sizeof(buffer) - len - HMAC_SIZE, req, AES_BLOCK_SIZE, &length))
                {
                    status = (length == AES_BLOCK_SIZE);
                }
            }
        }
    }

    return status;
}

static bool response(const uint8_t *res)
{
    size_t len, length;
    bool status = false;
    uint8_t buffer[sizeof(session_id) + sizeof(seq_number) + AES_IV_SIZE + AES_BLOCK_SIZE + HMAC_SIZE];

    len = sizeof(session_id) + sizeof(seq_number);

    if (PSA_SUCCESS == psa_cipher_encrypt(server_key, PSA_ALG_CBC_NO_PADDING, res, AES_BLOCK_SIZE, buffer + len, sizeof(buffer) - len, &length))
    {
        length = sizeof(buffer) - HMAC_SIZE;

        if (PSA_SUCCESS == psa_mac_compute(hmac_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, length, buffer + length, HMAC_SIZE, &len))
        {
            status = (sizeof(buffer) == write(connfd, buffer, sizeof(buffer)));
        }
    }

    return status;
}

int main(void)
{
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);

    const char *pers = "server";
    if (0 != mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers)))
    {
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_entropy_free(&entropy);
        exit(EXIT_FAILURE);
    }

    assert(hexstr_to_bin(STRINGIFY(HSECRET), SECRET));

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    assert(sockfd >= 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((0 == bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr))) && (0 == listen(sockfd, 1)))
    {
        if ((PSA_SUCCESS == psa_crypto_init()) && (PSA_SUCCESS == psa_import_key_from_file("server_keys.pem", &server_private_key,
                                                                                           PSA_KEY_TYPE_RSA_KEY_PAIR, RSA_BITS,
                                                                                           PSA_KEY_USAGE_DECRYPT, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256))))
        {
            struct sockaddr_in cli = {0};
            socklen_t len = sizeof(struct sockaddr_in);

            while (1)
            {
                printf("Waiting on a connection ...\n");
                connfd = accept(sockfd, (struct sockaddr *)&cli, &len);

                if (connfd >= 0)
                {
                    printf("Server acccepted the client...\n");

                    if (establish_session(connfd))
                    {
                        uint8_t message[AES_BLOCK_SIZE];

                        while (1)
                        {
                            if (!request(message))
                            {
                                break;
                            }

                            for (size_t i = 0; i < sizeof(message); i++)
                            {
                                message[i] = toupper(message[i]);
                            }

                            if (!response(message))
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        printf("Session establish failed...\n");
                    }

                    shutdown(connfd, SHUT_RDWR);
                    close(connfd); // Close the connection
                }
            }
        }

        session_reset();
    }

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd); // Close the socket

    return EXIT_FAILURE;
}
