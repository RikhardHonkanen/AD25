/**
 * @file client.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of mbedTLS client.
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

#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

static int sockfd;
static mbedtls_ctr_drbg_context ctr_drbg;
static uint64_t session_id = 0, seq_number = 0;
static psa_key_handle_t server_public_key, client_key, server_key, hmac_key;

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

static bool establish_session(void)
{
    size_t len, length;
    bool status = false;
    uint8_t buffer[RSA_SIZE];
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

#if 1
    // Load a standard 2048-bit MODP group from RFC 3526 (https://www.rfc-editor.org/rfc/rfc3526)
    if (0 != mbedtls_mpi_read_string(&P, 16,
                                     "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
                                     "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
                                     "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
                                     "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
                                     "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D"
                                     "C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F"
                                     "83655D23DCA3AD961C62F356208552BB9ED529077096966D"
                                     "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B"
                                     "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9"
                                     "DE2BCBF6955817183995497CEA956AE515D2261898FA0510"
                                     "15728E5A8AACAA68FFFFFFFF"))

#else
    // Generate P as the prime number. It takes time ...
    if (0 != mbedtls_mpi_gen_prime(&P, RSA_BITS, MBEDTLS_MPI_GEN_PRIME_FLAG_DH, mbedtls_ctr_drbg_random, &ctr_drbg))
#endif
    {
        goto end;
    }

    if (0 != mbedtls_mpi_write_binary_le(&P, buffer, RSA_SIZE))
    {
        goto end;
    }

    if ((PSA_SUCCESS != psa_asymmetric_encrypt(server_public_key, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),
                                               buffer, RSA_SIZE / 2, (const uint8_t *)OAEP_SALT, sizeof(OAEP_SALT) - 1,
                                               transcript, RSA_SIZE, &len)))
    {
        goto end;
    }
    length = len;
    if (PSA_SUCCESS != psa_asymmetric_encrypt(server_public_key, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),
                                              buffer + RSA_SIZE / 2, RSA_SIZE / 2, (const uint8_t *)OAEP_SALT, sizeof(OAEP_SALT) - 1,
                                              transcript + RSA_SIZE, RSA_SIZE, &len))
    {
        goto end;
    }
    length += len;

    if ((length != 2 * RSA_SIZE) || (length != (size_t)write(sockfd, transcript, length)))
    {
        goto end;
    }

    if (RSA_SIZE != read(sockfd, transcript + 2 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    if (0 != mbedtls_mpi_read_binary_le(&SPK, transcript + 2 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    // Generator = 2
    if (0 != mbedtls_mpi_lset(&G, 2))
    {
        goto end;
    }

    // Generate private Q, compute public CPK = G^Q mod P
    if ((0 != mbedtls_mpi_fill_random(&Q, RSA_SIZE, mbedtls_ctr_drbg_random, &ctr_drbg)) ||
        (0 != mbedtls_mpi_exp_mod(&CPK, &G, &Q, &P, NULL)))
    {
        goto end;
    }

    if (0 != mbedtls_mpi_write_binary_le(&CPK, transcript + 3 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    if (RSA_SIZE != (size_t)write(sockfd, transcript + 3 * RSA_SIZE, RSA_SIZE))
    {
        goto end;
    }

    // Compute the shared secret. H = SPK ^ Q mod P
    if (0 != mbedtls_mpi_exp_mod(&H, &SPK, &Q, &P, NULL))
    {
        goto end;
    }

    // Write the shared secret into buffer
    if (0 != mbedtls_mpi_write_binary_le(&H, buffer, RSA_SIZE))
    {
        goto end;
    }

    if (PSA_SUCCESS != psa_hash_compute(PSA_ALG_SHA_256, (const uint8_t *)STRINGIFY(SECRET), strlen(STRINGIFY(SECRET)), master_secret, HASH_SIZE, &len))
    {
        goto end;
    }

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attr, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_bits(&attr, HMAC_SIZE * CHAR_BIT);
    psa_set_key_type(&attr, PSA_KEY_TYPE_HMAC);
    if (PSA_SUCCESS != psa_import_key(&attr, master_secret, HMAC_SIZE, &master_key))
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

    // Compute the hashed transcript mac using the client authentication key
    if (PSA_SUCCESS != psa_mac_compute(client_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, HASH_SIZE, transcript, HMAC_SIZE, &length))
    {
        goto end;
    }

    if (HMAC_SIZE != (size_t)write(sockfd, transcript, HMAC_SIZE))
    {
        goto end;
    }

    if (HMAC_SIZE != (size_t)read(sockfd, transcript, HMAC_SIZE))
    {
        goto end;
    }

    if (PSA_SUCCESS != psa_mac_verify(server_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, HASH_SIZE, transcript, HMAC_SIZE))
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
    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attr, PSA_ALG_CBC_NO_PADDING);
    psa_set_key_type(&attr, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attr, AES_SIZE * CHAR_BIT);
    if (PSA_SUCCESS != psa_import_key(&attr, secret_keys[0], AES_SIZE, &client_key))
    {
        goto end;
    }

    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_DECRYPT);
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
        psa_destroy_key(client_key);
        psa_destroy_key(server_key);
        psa_destroy_key(hmac_key);
        session_id = 0;
        seq_number = 0;
    }
    return status;
}

static bool request(const uint8_t *req, uint8_t *res)
{
    size_t len, length;
    bool status = false;
    uint8_t buffer[sizeof(session_id) + sizeof(seq_number) + AES_IV_SIZE + AES_BLOCK_SIZE + HMAC_SIZE];

    memcpy(buffer, &session_id, sizeof(session_id));
    length = sizeof(session_id);

    seq_number++;
    memcpy(buffer + length, &seq_number, sizeof(seq_number));
    length += sizeof(seq_number);

    if (PSA_SUCCESS == psa_cipher_encrypt(client_key, PSA_ALG_CBC_NO_PADDING, req, AES_BLOCK_SIZE, buffer + length, sizeof(buffer) - length, &len))
    {
        length += len;

        if (PSA_SUCCESS == psa_mac_compute(hmac_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, length, buffer + length, HMAC_SIZE, &len))
        {
            if (sizeof(buffer) == write(sockfd, buffer, sizeof(buffer)))
            {
                if (sizeof(buffer) == read(sockfd, buffer, sizeof(buffer)))
                {
                    if (PSA_SUCCESS == psa_mac_verify(hmac_key, PSA_ALG_HMAC(PSA_ALG_SHA_256), buffer, length, buffer + length, HMAC_SIZE))
                    {
                        if ((0 == memcmp(&session_id, buffer, sizeof(session_id))) && (0 == memcmp(&seq_number, buffer + sizeof(session_id), sizeof(seq_number))))
                        {
                            len = sizeof(session_id) + sizeof(seq_number);

                            if (PSA_SUCCESS == psa_cipher_decrypt(server_key, PSA_ALG_CBC_NO_PADDING, buffer + len, sizeof(buffer) - len - HMAC_SIZE, res, AES_BLOCK_SIZE, &length))
                            {
                                status = (length == AES_BLOCK_SIZE);
                            }
                        }
                    }
                }
            }
        }
    }

    return status;
}

int main(void)
{
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);

    const char *pers = "client";
    if (0 != mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers)))
    {
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_entropy_free(&entropy);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    assert(sockfd >= 0);

    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (0 == connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        if ((PSA_SUCCESS == psa_crypto_init()) && (PSA_SUCCESS == psa_import_key_from_file("server_public_key.pem",
                                                                                           &server_public_key, PSA_KEY_TYPE_RSA_PUBLIC_KEY, RSA_BITS,
                                                                                           PSA_KEY_USAGE_ENCRYPT, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256))))
        {
            if (establish_session())
            {
                uint8_t message[AES_BLOCK_SIZE];

                while (1)
                {
                    if (PSA_SUCCESS != psa_generate_random(message, sizeof(message)))
                    {
                        break;
                    }

                    printf("    Sent: ");
                    for (size_t i = 0; i < sizeof(message); i++)
                    {
                        message[i] = 'a' + (message[i] % 26);
                        putchar(message[i]);
                    }
                    printf("\n");

                    if (!request(message, message))
                    {
                        break;
                    }

                    printf("Received: ");
                    for (size_t i = 0; i < sizeof(message); i++)
                    {
                        putchar(message[i]);
                    }
                    printf("\n\n");

                    sleep(1);
                }
            }
            else
            {
                printf("Session establish failed...\n");
            }
        }

        psa_destroy_key(server_public_key);
        psa_destroy_key(server_key);
        psa_destroy_key(client_key);
        psa_destroy_key(hmac_key);
    }

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd); // Close the socket

    return EXIT_FAILURE;
}
