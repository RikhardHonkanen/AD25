#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <esp_mac.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_system.h>

#include <lwip/err.h>
#include <lwip/sys.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

#define MAX_LEN 32
#define SERVER_PORT 12345

#define ESP_WIFI_AP_IP "192.168.4.1"
#define ESP_WIFI_AP_MASK "255.255.255.0"
#define ESP_WIFI_AP_GATEWAY "192.168.4.1"

#define ESP_WIFI_PASS "1234567890"
#define ESP_WIFI_SSID "ESP32"
#define EPS_WIFI_MAX_CONN 1
#define ESP_WIFI_CHANNEL 1

static const char *TAG = "wifi_tcp_server";

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " joined, AID=%d", MAC2STR(event->mac), event->aid);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " left", MAC2STR(event->mac));
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

    wifi_softap_init();

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (sockfd >= 0)
    {
        struct sockaddr_in servaddr = {0};
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_family = AF_INET;

        // Binding newly created socket to given IP and verification
        if (0 == bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
        {
            if (0 == listen(sockfd, 1)) // only one connection
            {
                int connfd;
                struct sockaddr_in cli = {0};
                socklen_t length = sizeof(cli);

                while (1)
                {
                    // Accept and check the connection
                    connfd = accept(sockfd, (struct sockaddr *)&cli, &length);
                    if (connfd >= 0)
                    {
                        ESP_LOGI(TAG, "Server acccepted the client...");

                        size_t len;
                        char buffer[MAX_LEN + 1];

                        while (1)
                        {
                            bzero(buffer, sizeof(buffer));

                            // Read the message from client and store it in buffer
                            if (1 > read(connfd, buffer, MAX_LEN))
                            {
                                ESP_LOGE(TAG, "Failed to read: errno %d", errno);
                                break;
                            }

                            len = strlen(buffer);

                            for (size_t i = 0; i < len; i++)
                            {
                                buffer[i] = toupper(buffer[i]);
                            }

                            // Send buffer to client
                            if (len != write(connfd, buffer, len))
                            {
                                ESP_LOGE(TAG, "Failed to write: errno %d", errno);
                                break;
                            }
                        }

                        shutdown(connfd, SHUT_RDWR);
                        close(connfd); // Close the connection
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Failed to accept the connection: errno %d", errno);
                    }

                    sleep(1);
                }
            }
            else
            {
                ESP_LOGE(TAG, "Failed to listen to the port: errno %d", errno);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to bind servaddr to the socket: errno %d", errno);
        }

        // Close the socket
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
    }
    else
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }
}
