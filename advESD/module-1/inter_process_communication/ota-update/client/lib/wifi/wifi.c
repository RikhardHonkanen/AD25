#include "wifi.h"

#include <bootloader_random.h>
#include <esp_ota_ops.h>
#include <esp_random.h>
#include <esp_system.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_log.h>

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
#include <stdint.h>

static const char *TAG = "WiFi";
static volatile bool connected = false;
static const char *server_ip, *server_port;

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
        connected = true;
    }
}

static void over_the_air_update(void *param)
{
    (void)param;

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_net_context conn_ctx;
    static uint8_t buffer[4096];
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    assert(PSA_SUCCESS == psa_crypto_init());
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ssl_config_init(&conf);
    mbedtls_entropy_init(&entropy);

    uint8_t temp[32];
    bootloader_random_enable();
    for (size_t i = 0; i < sizeof(temp); i++)
    {
        temp[i] = esp_random() % 256;
    }
    bootloader_random_disable();

    assert(0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)temp, sizeof(temp)));
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    assert(0 == mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT));

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE); // It shall be MBEDTLS_SSL_VERIFY_REQUIRED in a real product

    while (1)
    {
        mbedtls_ssl_init(&ssl);

        if (0 == mbedtls_ssl_setup(&ssl, &conf))
        {
            mbedtls_net_init(&conn_ctx);

            if (0 == mbedtls_net_connect(&conn_ctx, server_ip, server_port, MBEDTLS_NET_PROTO_TCP))
            {
                mbedtls_ssl_set_bio(&ssl, &conn_ctx, mbedtls_net_send, NULL, mbedtls_net_recv_timeout);

                if (0 == mbedtls_ssl_handshake(&ssl))
                {
                    esp_ota_handle_t update_handle;
                    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
                    ESP_ERROR_CHECK(esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle));

                    int len;

                    while (1)
                    {
                        len = mbedtls_ssl_read(&ssl, buffer, sizeof(buffer));

                        if (len > 0)
                        {
                            ESP_ERROR_CHECK(esp_ota_write(update_handle, buffer, len));
                        }
                        else
                        {
                            if (len == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
                            {
                                esp_err_t status = esp_ota_end(update_handle);

                                if (status == ESP_OK)
                                {
                                    status = esp_ota_set_boot_partition(update_partition);
                                    if (status == ESP_OK)
                                    {
                                        ESP_LOGI(TAG, "Prepare to restart system!");
                                        esp_restart();
                                    }
                                    else
                                    {
                                        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(status));
                                    }
                                }
                                else if (status == ESP_ERR_OTA_VALIDATE_FAILED)
                                {
                                    ESP_LOGE(TAG, "Image validation failed, image is corrupted");
                                }
                                else
                                {
                                    ESP_LOGE(TAG, "esp_ota_end failed (%s)!", esp_err_to_name(status));
                                }
                            }
                            else
                            {
                                ESP_ERROR_CHECK(esp_ota_abort(update_handle));
                                ESP_LOGE(TAG, "mbedtls_ssl_read failed! error: %d (%s)", len, esp_err_to_name(len));
                            }

                            break;
                        }
                    }
                }
            }

            mbedtls_net_free(&conn_ctx);
        }

        mbedtls_ssl_free(&ssl);
    }

    mbedtls_psa_crypto_free();
    mbedtls_ssl_config_free(&conf);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
}

void wifi_connect(const char *ssid, const char *pass)
{
    assert((ssid != NULL) && (pass != NULL));

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    assert(NULL != esp_netif_create_default_wifi_sta());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    wifi_config_t wifi_config = {0};
    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid));
    memcpy(wifi_config.sta.password, pass, strlen(pass));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    while (!connected)
    {
        vTaskDelay(pdMS_TO_TICKS(250));
        ESP_LOGI(TAG, "Connecting ...");
    }
}

void wifi_ota_update_set_server(const char *ip, const char *port)
{
    assert((ip != NULL) && (port != NULL));

    server_ip = ip;
    server_port = port;

    assert(pdPASS == xTaskCreate(over_the_air_update, "Over The Air Update", 4096, NULL, 3, NULL));
}