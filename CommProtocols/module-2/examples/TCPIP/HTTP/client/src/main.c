#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <esp_mac.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_http_client.h>

#define ESP_WIFI_PASS "1234567890"
#define ESP_WIFI_SSID "ESP32"

#define SERVER_IP "192.168.4.1"

static const char *TAG = "wifi_http_client";

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
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_sta_init finished.");
}

static esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
    esp_err_t status = ESP_OK;

    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("Received: %.*s\n", evt->data_len, (const char *)evt->data);
        break;
    case HTTP_EVENT_ERROR:
        status = ESP_FAIL;
        break;
    default:
        break;
    }

    return status;
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

    wifi_sta_init();

    while (1)
    {
        esp_http_client_handle_t client;
        esp_http_client_config_t config = {
            .method = HTTP_METHOD_GET,
            .event_handler = client_event_get_handler,
        };

        config.url = "http://" SERVER_IP "/person";
        client = esp_http_client_init(&config);
        esp_http_client_perform(client);
        esp_http_client_cleanup(client);

        config.url = "http://" SERVER_IP "/person?fname=Eva&lname=Larsson";
        client = esp_http_client_init(&config);
        esp_http_client_perform(client);
        esp_http_client_cleanup(client);

        sleep(1);
    }
}
