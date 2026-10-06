/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An HTTPS GET Example using plain mbedTLS sockets
 * @version 0.1
 * @date 2025-02-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <freertos/FreeRTOS.h>
#include <bootloader_random.h>
#include <freertos/task.h>
#include <esp_random.h>
#include <esp_system.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <string.h>

#include <mbedtls/net_sockets.h>
#include <mbedtls/esp_debug.h>
#include <mbedtls/platform.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <esp_crt_bundle.h>
#include <mbedtls/error.h>
#include <mbedtls/ssl.h>

// https://www.digicert.com/kb/digicert-root-certificates.htm

#define WIFI_SSID "MaxPlus"
#define WIFI_PASS "SV21TRC3556"

#define WEB_PORT "443"
#define WEB_SERVER "www.howsmyssl.com"
#define WEB_URL "https://www.howsmyssl.com/a/check"

static const char *TAG = "https_client";

static const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
                             "Host: " WEB_SERVER "\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "Retrying to connect to the AP");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

static void wifi_sta_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif != NULL);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

static void https_get_task(void *pvParameters)
{
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_ssl_context ssl;
    mbedtls_x509_crt cacert;
    mbedtls_ssl_config conf;
    uint8_t buffer[512];

    mbedtls_ssl_config_init(&conf);

    mbedtls_ssl_init(&ssl);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    ESP_LOGI(TAG, "Seeding the random number generator");
    bootloader_random_enable();
    for (size_t i = 0; i < MBEDTLS_CTR_DRBG_ENTROPY_LEN; i++)
    {
        buffer[i] = esp_random() % 256;
    }
    bootloader_random_disable();

    assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)buffer, MBEDTLS_CTR_DRBG_ENTROPY_LEN));

    ESP_LOGI(TAG, "Attaching the certificate bundle...");
    ESP_ERROR_CHECK(esp_crt_bundle_attach(&conf));

    ESP_LOGI(TAG, "Setting hostname for TLS session...");

    /* Hostname set here should match CN in server certificate */
    assert(0 == mbedtls_ssl_set_hostname(&ssl, WEB_SERVER));

    ESP_LOGI(TAG, "Setting up the SSL/TLS structure...");

    assert(0 == mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT));

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    assert(0 == mbedtls_ssl_setup(&ssl, &conf));

    int ret, flags, len;
    mbedtls_net_context server_fd;

    while (1)
    {
        mbedtls_net_init(&server_fd);

        ESP_LOGI(TAG, "Connecting to %s:%s...", WEB_SERVER, WEB_PORT);

        if (0 == mbedtls_net_connect(&server_fd, WEB_SERVER, WEB_PORT, MBEDTLS_NET_PROTO_TCP))
        {
            ESP_LOGI(TAG, "Connected.");
            mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

            ESP_LOGI(TAG, "Performing the SSL/TLS handshake...");

            while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
            {
                if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
                {
                    ESP_LOGE(TAG, "mbedtls_ssl_handshake failed");
                    goto exit;
                }
            }

            ESP_LOGI(TAG, "Verifying peer X.509 certificate...");

            flags = mbedtls_ssl_get_verify_result(&ssl);

            if (flags == 0)
            {
                ESP_LOGI(TAG, "Certificate verified.");
            }
            else
            {
                ESP_LOGW(TAG, "Failed to verify peer certificate!");

                bzero(buffer, sizeof(buffer));
                mbedtls_x509_crt_verify_info((char *)buffer, sizeof(buffer), "  ! ", flags);
                ESP_LOGW(TAG, "verification info: %s", (const char *)buffer);

                goto exit;
            }

            ESP_LOGI(TAG, "Cipher suite is %s", mbedtls_ssl_get_ciphersuite(&ssl));

            ESP_LOGI(TAG, "Writing HTTP request...");

            size_t length = 0;
            do
            {
                ret = mbedtls_ssl_write(&ssl, (const uint8_t *)REQUEST + length, strlen(REQUEST) - length);
                if (ret >= 0)
                {
                    ESP_LOGI(TAG, "%d bytes written", ret);
                    length += ret;
                }
                else if (ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_WANT_READ)
                {
                    ESP_LOGE(TAG, "mbedtls_ssl_write returned -0x%x", -ret);
                    goto exit;
                }
            } while (length < strlen(REQUEST));

            ESP_LOGI(TAG, "Reading HTTP response...");

            do
            {
                len = sizeof(buffer) - 1;
                bzero(buffer, sizeof(buffer));

                ret = mbedtls_ssl_read(&ssl, buffer, len);

                if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
                {
                    continue;
                }

                if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
                {
                    break;
                }

                if (ret < 0)
                {
                    ESP_LOGE(TAG, "mbedtls_ssl_read returned failed");
                    break;
                }

                if (ret == 0)
                {
                    ESP_LOGI(TAG, "connection closed");
                    break;
                }

                len = ret;
                for (int i = 0; i < len; i++)
                {
                    putchar(buffer[i]);
                }
            } while (1);

            mbedtls_ssl_close_notify(&ssl);
        }
        else
        {
            ESP_LOGE(TAG, "mbedtls_net_connect failed");
        }

    exit:
        mbedtls_ssl_session_reset(&ssl);
        mbedtls_net_free(&server_fd);

        putchar('\n');

        static int request_count;
        ESP_LOGI(TAG, "Completed %d requests", ++request_count);
        printf("Minimum free heap size: %lu bytes\n", esp_get_minimum_free_heap_size());

        for (int countdown = 10; countdown >= 0; countdown--)
        {
            ESP_LOGI(TAG, "%d ...", countdown);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        ESP_LOGI(TAG, "Starting again!");
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_IDF Version: %s", esp_get_idf_version());

    wifi_sta_init();

    assert(pdTRUE == xTaskCreate(&https_get_task, "https_get_task", 8192, NULL, 5, NULL));
}
