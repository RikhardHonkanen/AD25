#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utility.h"
#include <esp_mac.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <lwip/sockets.h>
#include <esp_http_server.h>

#define ESP_WIFI_AP_IP "192.168.4.1"
#define ESP_WIFI_AP_MASK "255.255.255.0"
#define ESP_WIFI_AP_GATEWAY "192.168.4.1"

#define ESP_WIFI_PASS "1234567890"
#define ESP_WIFI_SSID "ESP32"
#define EPS_WIFI_MAX_CONN 2
#define ESP_WIFI_CHANNEL 1

#define HTTP_QUERY_KEY_MAX_LEN 64

static esp_err_t person_handler(httpd_req_t *req);

static httpd_handle_t server = NULL;
static const char *TAG = "wifi_ap_http_server";

static const httpd_uri_t person = {
    .uri = "/person", /* http://<server-ip>/person */
    .method = HTTP_GET,
    .handler = person_handler,
    .user_ctx = "<h1>Hello World!</h1>",
};

static esp_err_t person_handler(httpd_req_t *req)
{
    size_t len;
    char buffer[HTTP_QUERY_KEY_MAX_LEN] = {0};

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (len > 1)
    {
        /* Copy null terminated value string into buffer */
        if (ESP_OK == httpd_req_get_hdr_value_str(req, "Host", buffer, len))
        {
            ESP_LOGI(TAG, "Found header => Host: %s", buffer);
        }
    }

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    len = httpd_req_get_url_query_len(req) + 1;

    if (len > 1)
    {
        char query[HTTP_QUERY_KEY_MAX_LEN] = {0};

        if (ESP_OK == httpd_req_get_url_query_str(req, query, len))
        {
            ESP_LOGI(TAG, "Found URL query => %s", query);
            char param[HTTP_QUERY_KEY_MAX_LEN], dec_param[HTTP_QUERY_KEY_MAX_LEN];

            /* Get value of expected key from query string */
            // http://<server-ip>/person?fname=Stefan&lname=Larsson

            bzero(param, sizeof(param));
            bzero(dec_param, sizeof(dec_param));
            if (ESP_OK == httpd_query_key_value(query, "fname", param, sizeof(param)))
            {
                ESP_LOGI(TAG, "Found URL query parameter => fname=%s", param);
                utility_uri_decode(dec_param, param, strnlen(param, HTTP_QUERY_KEY_MAX_LEN));
                ESP_LOGI(TAG, "Decoded query parameter => %s", dec_param);

                strcpy(buffer, "<h1>Hello, ");
                strcat(buffer, param);
                strcat(buffer, " ");

                bzero(param, sizeof(param));
                bzero(dec_param, sizeof(dec_param));
                if (ESP_OK == httpd_query_key_value(query, "lname", param, sizeof(param)))
                {
                    ESP_LOGI(TAG, "Found URL query parameter => lname=%s", param);
                    utility_uri_decode(dec_param, param, strnlen(param, HTTP_QUERY_KEY_MAX_LEN));
                    ESP_LOGI(TAG, "Decoded query parameter => %s", dec_param);

                    strcat(buffer, param);
                    strcat(buffer, "!</h1>");
                }
                else
                {
                    strcpy(buffer, "<h1 style=\"color:red;\">Invalid Request!</h1>");
                }
            }
            else
            {
                strcpy(buffer, "<h1 style=\"color:red;\">Invalid Request!</h1>");
            }
        }
        else
        {
            strcpy(buffer, req->user_ctx);
        }
    }
    else
    {
        strcpy(buffer, req->user_ctx);
    }

    /* Send response with custom headers and body */
    return httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t error_handler(httpd_req_t *req, httpd_err_code_t error)
{
    const char *resp_str = "<h1>Page Not Found</h1>";
    ESP_ERROR_CHECK(httpd_resp_set_status(req, "404 Not Found"));
    return httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
}

static void webserver_start(void)
{
    server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (ESP_OK == httpd_start(&server, &config))
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        ESP_ERROR_CHECK(httpd_register_uri_handler(server, &person));
        ESP_ERROR_CHECK(httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, &error_handler));
    }
    else
    {
        server = NULL;
    }
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " joined, AID=%d", MAC2STR(event->mac), event->aid);

        if (server == NULL)
        {
            ESP_LOGI(TAG, "Starting webserver");
            webserver_start();
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " left", MAC2STR(event->mac));

        if (server != NULL)
        {
            ESP_LOGI(TAG, "Stopping webserver");
            if (ESP_OK == httpd_stop(server))
            {
                server = NULL;
            }
            else
            {
                ESP_LOGE(TAG, "Failed to stop the http server");
            }
        }
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
}
