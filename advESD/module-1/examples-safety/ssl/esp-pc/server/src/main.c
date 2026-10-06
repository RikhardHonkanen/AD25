/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of mbedTLS server
 * @version 0.1
 * @date 2025-08-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <esp_mac.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_random.h>
#include <bootloader_random.h>

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

#define ESP_WIFI_AP_IP "192.168.4.1"
#define ESP_WIFI_AP_MASK "255.255.255.0"
#define ESP_WIFI_AP_GATEWAY "192.168.4.1"

#define ESP_WIFI_PASS "1234567890"
#define ESP_WIFI_SSID "ESP32"
#define EPS_WIFI_MAX_CONN 1
#define ESP_WIFI_CHANNEL 1

#define BUFLEN 16
#define PORT "12345"

static mbedtls_net_context server_ctx, client_ctx;
static volatile bool wifi_connected = false;
static const char *TAG = "mbedtls_server";
static mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_entropy_context entropy;
static mbedtls_ssl_context ssl;
static mbedtls_ssl_config conf;
static mbedtls_x509_crt srvcert;
static mbedtls_pk_context pkey;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " joined, AID=%d", MAC2STR(event->mac), event->aid);

        mbedtls_ssl_init(&ssl);
        mbedtls_pk_init(&pkey);
        mbedtls_net_init(&client_ctx);
        mbedtls_net_init(&server_ctx);
        mbedtls_ssl_config_init(&conf);
        mbedtls_x509_crt_init(&srvcert);
        mbedtls_entropy_init(&entropy);
        mbedtls_ctr_drbg_init(&ctr_drbg);
        assert(PSA_SUCCESS == psa_crypto_init());

        uint8_t buffer[BUFLEN];
        bootloader_random_enable();
        for (size_t i = 0; i < sizeof(buffer); i++)
        {
            buffer[i] = esp_random() % 256;
        }
        bootloader_random_disable();

        assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)buffer, sizeof(buffer)));
        mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

        extern const unsigned char servercert_start[] asm("_binary_srvrcert_pem_start");
        extern const unsigned char servercert_end[] asm("_binary_srvrcert_pem_end");
        extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
        extern const unsigned char prvtkey_pem_end[] asm("_binary_prvtkey_pem_end");

        assert(0 == mbedtls_x509_crt_parse(&srvcert, servercert_start, servercert_end - servercert_start));
        assert(0 == mbedtls_pk_parse_key(&pkey, prvtkey_pem_start, prvtkey_pem_end - prvtkey_pem_start, NULL, 0, mbedtls_entropy_func, NULL));

        mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
        assert(0 == mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey));

        assert(0 == mbedtls_net_bind(&server_ctx, ESP_WIFI_AP_IP, PORT, MBEDTLS_NET_PROTO_TCP));

        assert(0 == mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT));

        assert(0 == mbedtls_ssl_setup(&ssl, &conf));

        wifi_connected = true;
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " left", MAC2STR(event->mac));
        wifi_connected = false;
    }
    else
    {
        ;
    }
}

static void wifi_softap_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t *esp_netif = esp_netif_create_default_wifi_ap();
    assert(esp_netif != NULL);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));

    /**** Static IP Configuration  ****/
    esp_netif_ip_info_t ip_info = {0};
    assert(1 == inet_pton(AF_INET, ESP_WIFI_AP_IP, &ip_info.ip.addr));
    assert(1 == inet_pton(AF_INET, ESP_WIFI_AP_GATEWAY, &ip_info.gw.addr));
    assert(1 == inet_pton(AF_INET, ESP_WIFI_AP_MASK, &ip_info.netmask.addr));
    ESP_ERROR_CHECK(esp_netif_dhcps_stop(esp_netif));
    ESP_ERROR_CHECK(esp_netif_set_ip_info(esp_netif, &ip_info));
    ESP_ERROR_CHECK(esp_netif_dhcps_start(esp_netif));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .channel = ESP_WIFI_CHANNEL,
            .password = ESP_WIFI_PASS,
            .max_connection = EPS_WIFI_MAX_CONN,
            .authmode = (0 == strlen(ESP_WIFI_PASS)) ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "ESP32 SoftAP started");
    ESP_LOGI(TAG, "SSID:%s password:%s", ESP_WIFI_SSID, ESP_WIFI_PASS);
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_IDF Version: %s", esp_get_idf_version());

    wifi_softap_init();

    while (!wifi_connected)
    {
        sleep(1);
    }

    uint8_t buffer[BUFLEN + 1] = {0};

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
            if (0 >= mbedtls_ssl_read(&ssl, buffer, BUFLEN))
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
}
