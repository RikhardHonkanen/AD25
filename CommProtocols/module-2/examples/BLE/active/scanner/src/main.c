/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of active scanning using NimBLE
 *        Look at: https://mynewt.apache.org/latest/network/index.html
 *        In Menuconfig change the following settings:
 *           1. (Top) → Component config → Enable Bluetooth
 *                    - Host → NimBLE - BLE only
 *           2. (Top) → Component config → Bluetooth → NimBLE Options → Enable BLE 5 feature
 *                    - Disable extended advertising
 *                    - Disable extended scanning
 *
 * @version 0.1
 * @date 2025-05-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/ble.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"

#define TAG "Example"

static int ble_gap_event_handler(struct ble_gap_event *event, void *)
{
    if (event->type == BLE_GAP_EVENT_DISC)
    {
        uint8_t index = 0;
        int8_t pwr_lvl = 0;
        char uri[32] = {0};
        char name[32] = {0};
        char manufacturer[32] = {0};

        while (index < event->disc.length_data)
        {
            uint8_t field_len = event->disc.data[index];

            if (field_len == 0 || (index + field_len >= event->disc.length_data))
            {
                break;
            }

            uint8_t field_type = event->disc.data[index + 1];

            switch (field_type)
            {
            case BLE_HS_ADV_TYPE_INCOMP_NAME: // Shortened Local Name
            case BLE_HS_ADV_TYPE_COMP_NAME:   // Complete Local Name
            {
                size_t name_len = field_len - 1;
                if (name_len >= sizeof(name))
                {
                    name_len = sizeof(name) - 1;
                }
                memcpy(name, &event->disc.data[index + 2], name_len);
                name[name_len] = '\0';
            }
            break;

            case BLE_HS_ADV_TYPE_TX_PWR_LVL: /* TX Power */
                pwr_lvl = event->disc.data[index + 2];
                break;

            case BLE_HS_ADV_TYPE_MFG_DATA: /* Manufacturer Specific Data */
                for (int i = 0; i < field_len - 1 - sizeof(uint16_t); i++)
                {
                    manufacturer[i] = event->disc.data[index + 2 + sizeof(uint16_t) + i];
                }
                break;

            case BLE_HS_ADV_TYPE_URI:
                switch (event->disc.data[index + 2])
                {
                case BLE_EDDYSTONE_URL_SCHEME_HTTPS_WWW:
                    strcpy(uri, "https://www.");
                    break;
                case BLE_EDDYSTONE_URL_SCHEME_HTTP_WWW:
                    strcpy(uri, "http://www.");
                    break;
                case BLE_EDDYSTONE_URL_SCHEME_HTTPS:
                    strcpy(uri, "https://");
                    break;
                default:
                    strcpy(uri, "http://");
                    break;
                }

                for (int i = 0, start = strlen(uri); i < field_len - 1; i++)
                {
                    uri[start + i] = event->disc.data[index + 3 + i];
                }
                break;

            default:
                break;
            }

            index += field_len + 1;
        }

        // Determine whether this is a scan response or advertisement
        if (event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP)
        {
            if (manufacturer[0] != 0)
            {
                printf(", Response: ");
                for (int i = 0; manufacturer[i] != 0; i++)
                {
                    if (isprint((int)manufacturer[i]))
                    {
                        putchar(manufacturer[i]);
                    }
                    else
                    {
                        printf(" 0x%02x", manufacturer[i]);
                    }
                }
            }

            if (uri[0] != 0)
            {
                printf(" (%s)", uri);
            }
        }
        else
        {
            printf("\nDevice: Address = %02X:%02X:%02X:%02X:%02X:%02X",
                   event->disc.addr.val[5], event->disc.addr.val[4], event->disc.addr.val[3],
                   event->disc.addr.val[2], event->disc.addr.val[1], event->disc.addr.val[0]);

            // RSSI: Received Signal Strength Indicator
            // Typically expressed in decibels relative to 1 milliwatt (dBm).
            printf(", RSSI = %04d dBm", event->disc.rssi);

            if (pwr_lvl > 0)
            {
                printf(", TX Power = %02d dBm", pwr_lvl);
            }

            if (0 < strlen(name))
            {
                printf(", Name = %s", name);
            }
        }
    }

    return 0;
}

static void start_active_scan(void)
{
    struct ble_gap_disc_params disc_params = {0};

    disc_params.passive = 0;
    disc_params.itvl = BLE_GAP_ADV_ITVL_MS(1000);
    disc_params.window = BLE_GAP_ADV_ITVL_MS(100);
    disc_params.filter_duplicates = 0;
    disc_params.filter_policy = BLE_HCI_SCAN_FILT_NO_WL;

    assert(0 == ble_gap_disc(BLE_ADDR_PUBLIC, BLE_HS_FOREVER, &disc_params, ble_gap_event_handler, NULL));

    ESP_LOGI(TAG, "Passive scan started");
}

// on_stack_reset is called when host resets BLE stack due to errors
static void on_stack_reset(int reason)
{
    /* On reset, print reset reason to console */
    ESP_LOGI(TAG, "NimBLE stack reset, reset reason: %d", reason);
}

// on_stack_sync is called when host has synced with controller
// Initialize and start advertising
static void on_stack_sync(void)
{
    assert(0 == ble_hs_util_ensure_addr(0)); /* Configure the device with a Bluetooth address */

    start_active_scan();
}

void app_main(void)
{
    /* NVS flash initialization */
    esp_err_t status = nvs_flash_init();
    if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);

    if (status == ESP_OK)
    {
        status = nimble_port_init(); /* NimBLE host stack initialization */

        if (status == ESP_OK)
        {
            /* NimBLE host configuration initialization */
            ble_hs_cfg.sync_cb = on_stack_sync;
            ble_hs_cfg.reset_cb = on_stack_reset;
            ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

            ESP_LOGI(TAG, "Start nimble host!");
            nimble_port_run(); /* This function will not return until nimble_port_stop() is called */
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize nimble stack, error code: %d ", status);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to initialize nvs flash, error code: %d ", status);
    }
}