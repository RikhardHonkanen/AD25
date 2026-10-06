#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mbedtls/md.h>
#include <esp_random.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>
#include <mbedtls/gcm.h>
#include <mbedtls/sha256.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <bootloader_random.h>

#define AES_SIZE 32
#define HASH_SIZE 32
#define RSA_SIZE 128
#define EXPONENT 65537
#define HMAC_KEY "secretKey"
#define MSGLEN (RSA_SIZE / 4)

static void print(const uint8_t *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		printf("%02X ", data[i]);
	}
	printf("\n");
}

void app_main(void)
{
	uint8_t enc_iv[16] = {0};
	uint8_t dec_iv[16] = {0};
	uint8_t aes_iv[12] = {0};
	uint8_t aes_tag[16] = {0};
	mbedtls_aes_context aes_ctx;
	mbedtls_gcm_context gcm_ctx;
	mbedtls_rsa_context rsa_ctx;
	mbedtls_md_context_t hmac_ctx;
	mbedtls_entropy_context entropy;
	uint8_t aes_key[AES_SIZE] = {0};
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_sha256_context sha256_ctx;
	const uint8_t AAD[] = {0x01, 0x02, 0x03, 0x04}; // Optional AAD (Additional Authentication Data)

	// RNG Initialization
	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	bootloader_random_enable();
	for (size_t i = 0; i < sizeof(aes_key); i++)
	{
		aes_key[i] = esp_random() % 256;
	}
	bootloader_random_disable();

	assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, aes_key, sizeof(aes_key)));

	// SHA-256
	mbedtls_sha256_init(&sha256_ctx);

	// HMAC-SHA256
	mbedtls_md_init(&hmac_ctx);
	assert(0 == mbedtls_md_setup(&hmac_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1));

	// AES-256
	assert(0 == mbedtls_ctr_drbg_random(&ctr_drbg, aes_key, sizeof(aes_key)));

	mbedtls_aes_init(&aes_ctx);
	assert(0 == mbedtls_aes_setkey_enc(&aes_ctx, aes_key, sizeof(aes_key) * CHAR_BIT));

	mbedtls_gcm_init(&gcm_ctx);
	assert(0 == mbedtls_gcm_setkey(&gcm_ctx, MBEDTLS_CIPHER_ID_AES, aes_key, sizeof(aes_key) * CHAR_BIT));

	// RSA initialization and key generation
	mbedtls_rsa_init(&rsa_ctx);
	assert(0 == mbedtls_rsa_gen_key(&rsa_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, RSA_SIZE * CHAR_BIT, EXPONENT));

	char message[RSA_SIZE] = {0};
	uint8_t cipher[RSA_SIZE] = {0};

	while (1)
	{
		mbedtls_ctr_drbg_random(&ctr_drbg, (uint8_t *)message, MSGLEN);
		for (size_t i = 0; i < MSGLEN; i++)
		{
			message[i] = ((0 == (message[i] % 2)) ? 'a' : 'A') + (message[i] % 26);
		}
		printf("\n\nMessage    : %s\n", message);

#if 0
        /**************************************************************************************/
        /*********************************** SHA-256 ******************************************/
        /**************************************************************************************/
        uint8_t hash[HASH_SIZE] = {0};
        assert(0 == mbedtls_sha256_starts(&sha256_ctx, 0));
        assert(0 == mbedtls_sha256_update(&sha256_ctx, (const uint8_t *)message, MSGLEN));
        assert(0 == mbedtls_sha256_finish(&sha256_ctx, hash));
        printf("SHA-256    : ");
        print(hash, sizeof(hash));
#endif

#if 0
        /**************************************************************************************/
        /********************************* HMAC-SHA256 ****************************************/
        /**************************************************************************************/
        uint8_t hmac[HASH_SIZE] = {0};
        mbedtls_md_hmac_starts(&hmac_ctx, (const uint8_t *)HMAC_KEY, strlen(HMAC_KEY));
        mbedtls_md_hmac_update(&hmac_ctx, (const uint8_t *)message, MSGLEN);
        mbedtls_md_hmac_finish(&hmac_ctx, hmac);
        printf("HMAC-SHA256: ");
        print(hmac, sizeof(hmac));
#endif

#if 0
		/**************************************************************************************/
		/********************************* AES-256 CBC ****************************************/
		/******************************** [IV | CIPHER ] **************************************/
		/**************************************************************************************/
		printf("AES-Key    : ");
		print(aes_key, sizeof(aes_key));

		mbedtls_ctr_drbg_random(&ctr_drbg, enc_iv, sizeof(enc_iv));
		memcpy(dec_iv, enc_iv, sizeof(dec_iv)); // enc_iv and dec_iv shall be the same

		// MSGLEN shall be a multiple of 16
		assert(0 == mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, MSGLEN, enc_iv, (const uint8_t *)message, cipher));
		printf("CBC-CIPHER : ");
		print(cipher, MSGLEN);

		memset(message, 0, sizeof(message));
		assert(0 == mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, MSGLEN, dec_iv, cipher, (uint8_t *)message));
		printf("Message    : %s\n", message);
#endif

#if 0
		/**************************************************************************************/
		/*********************** AES-GCM ENCRYPTION / DECRYPTION ******************************/
		/***************************** [IV | CIPHER | TAG ] ***********************************/
		/**************************************************************************************/
		printf("AES-Key    : ");
		print(aes_key, sizeof(aes_key));

		memset(cipher, 0, sizeof(cipher));
		mbedtls_ctr_drbg_random(&ctr_drbg, aes_iv, sizeof(aes_iv));
		assert(0 == mbedtls_gcm_crypt_and_tag(&gcm_ctx, MBEDTLS_GCM_ENCRYPT, MSGLEN, aes_iv, sizeof(aes_iv),
											  AAD, sizeof(AAD), (const uint8_t *)message, cipher, sizeof(aes_tag), aes_tag));
		printf("GCM CIPHER : ");
		print(cipher, MSGLEN);
		printf("GCM TAG    : ");
		print(aes_tag, sizeof(aes_tag));
		memset(message, 0, sizeof(message));
		assert(0 == mbedtls_gcm_auth_decrypt(&gcm_ctx, MSGLEN, aes_iv, sizeof(aes_iv),
											 AAD, sizeof(AAD), aes_tag, sizeof(aes_tag), cipher, (uint8_t *)message));
		printf("Message    : %s\n", message);
#endif

#if 0
        /**************************************************************************************/
        /*************************** RSA PUB ENC, PRIV DEC ************************************/
        /**************************************************************************************/
        assert(0 == mbedtls_rsa_public(&rsa_ctx, (const uint8_t *)message, cipher));
        printf("RSA-CIPHER:  ");
        print(cipher, sizeof(cipher));

        memset(message, 0, sizeof(message));
        assert(0 == mbedtls_rsa_private(&rsa_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, cipher, (uint8_t *)message));
        printf("Message    : %s\n", message);
#endif

#if 0
        /**************************************************************************************/
        /*************************** RSA PRIV ENC, PUB DEC ************************************/
        /**************************************************************************************/
        assert(0 == mbedtls_rsa_private(&rsa_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, (uint8_t *)message, cipher));
        printf("RSA-CIPHER:  ");
        print(cipher, sizeof(cipher));

        memset(message, 0, sizeof(message));
        assert(0 == mbedtls_rsa_public(&rsa_ctx, cipher, (uint8_t *)message));
        printf("Message    : %s\n", message);
#endif

#if 0
        /**************************************************************************************/
        /***************************** RSA Export/Import Raw **********************************/
        /**************************************************************************************/
        uint8_t public_key[RSA_SIZE] = {0};
        uint8_t private_key[RSA_SIZE] = {0};
        uint8_t exp[sizeof(EXPONENT)] = {0};

        assert(0 == mbedtls_rsa_export_raw(&rsa_ctx, public_key, sizeof(public_key), NULL, 0, NULL, 0, private_key, sizeof(private_key), exp, sizeof(exp)));
        printf("Public : ");
        print(public_key, sizeof(public_key));

        printf("Private: ");
        print(private_key, sizeof(private_key));

        printf("Exponent: ");
        print(exp, sizeof(exp));

        mbedtls_rsa_context temp;
        mbedtls_rsa_init(&temp);

        assert(0 == mbedtls_rsa_import_raw(&temp, public_key, sizeof(public_key), NULL, 0, NULL, 0, NULL, 0, exp, sizeof(exp)));

        assert(0 == mbedtls_rsa_public(&temp, (const uint8_t *)message, cipher));
        printf("RSA-CIPHER:  ");
        print(cipher, sizeof(cipher));

        memset(message, 0, sizeof(message));
        assert(0 == mbedtls_rsa_private(&rsa_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, cipher, (uint8_t *)message));
        printf("Message    : %s\n", message);

        mbedtls_rsa_free(&temp);
#endif

#if 0
        /**************************************************************************************/
        /***************************** RSA Export/Import Raw **********************************/
        /**************************************************************************************/
        typedef struct
        {
            uint8_t P[RSA_SIZE / 2];
            uint8_t Q[RSA_SIZE / 2];
            uint8_t N[RSA_SIZE];
            uint8_t D[RSA_SIZE];
            uint8_t E[sizeof(EXPONENT)];
        } rsa_keys_t;
        rsa_keys_t keys;

        assert(0 == mbedtls_rsa_export_raw(&rsa_ctx, keys.N, sizeof(keys.N), keys.P, sizeof(keys.P), keys.Q, sizeof(keys.Q), keys.D, sizeof(keys.D), keys.E, sizeof(keys.E)));
        print((uint8_t *)&keys, sizeof(keys));

        mbedtls_rsa_context temp;
        mbedtls_rsa_init(&temp);
        assert(0 == mbedtls_rsa_import_raw(&temp, keys.N, sizeof(keys.N), keys.P, sizeof(keys.P), keys.Q, sizeof(keys.Q), keys.D, sizeof(keys.D), keys.E, sizeof(keys.E)));

        assert(0 == mbedtls_rsa_public(&temp, (const uint8_t *)message, cipher));

        memset(message, 0, sizeof(message));
        assert(0 == mbedtls_rsa_private(&rsa_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, cipher, (uint8_t *)message));
        printf("Message    : %s\n", message);
#endif

		sleep(2);
	}
}
