/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of advertising + scan response as a connectable BLE device using NimBLE
 *        Install nRF Connect on your phone and scan & connect
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
#include <string.h>
#include <stdbool.h>
#include "esp_bt.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/ble.h"
#include "host/ble_hs.h"
#include "driver/gpio.h"
#include "host/util/util.h"
#include "host/ble_eddystone.h"
#include "nimble/nimble_port.h"
#include "services/gap/ble_svc_gap.h"

#define INTERVAL 50
#define TAG "Example"
#define DEVICE_NAME "ESP32"

// BLE defines appearance values to help remote devices understand what type of device they're interacting with.
// Look at page 30 of https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf
#define BLE_GAP_APPEARANCE_GENERIC_TAG 0x0200

static void start_advertising(void);

static uint8_t addr_type;

static void print_conn_desc(struct ble_gap_conn_desc *desc)
{
    char addr_str[18] = {0};

    ESP_LOGI(TAG, "Connection handle: %d", desc->conn_handle);

    /* Local ID address */
    sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X", desc->our_id_addr.val[5], desc->our_id_addr.val[4],
            desc->our_id_addr.val[3], desc->our_id_addr.val[2], desc->our_id_addr.val[1], desc->our_id_addr.val[0]);
    ESP_LOGI(TAG, "Device id address: type = %d, value = %s", desc->our_id_addr.type, addr_str);

    /* Peer ID address */
    sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X", desc->peer_id_addr.val[5], desc->peer_id_addr.val[4],
            desc->peer_id_addr.val[3], desc->peer_id_addr.val[2], desc->peer_id_addr.val[1], desc->peer_id_addr.val[0]);
    ESP_LOGI(TAG, "Peer id address: type = %d, value = %s", desc->peer_id_addr.type, addr_str);

    /* Connection info */
    ESP_LOGI(TAG,
             "conn_itvl = %d, conn_latency = %d, supervision_timeout = %d, "
             "encrypted = %d, authenticated = %d, bonded = %d\n",
             desc->conn_itvl, desc->conn_latency, desc->supervision_timeout,
             desc->sec_state.encrypted, desc->sec_state.authenticated,
             desc->sec_state.bonded);
}

static int gap_event_handler(struct ble_gap_event *event, void *)
{
    int status = 0;
    struct ble_gap_conn_desc desc;

    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT: /* Connect event */
        ESP_LOGI(TAG, "Connection %s; status = %d", event->connect.status == 0 ? "established" : "failed", event->connect.status);
        if (event->connect.status == 0) /* Connection succeeded */
        {
            status = ble_gap_conn_find(event->connect.conn_handle, &desc); /* Check connection handle */
            if (status == 0)
            {
                ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, 1));

                print_conn_desc(&desc);

                struct ble_gap_upd_params params = {
                    .itvl_min = desc.conn_itvl,
                    .itvl_max = desc.conn_itvl,
                    .latency = 3,                                    // The number of connection events the peripheral can skip to save power.
                    .supervision_timeout = desc.supervision_timeout, // The maximum time between two successful data exchanges before
                                                                     // the BLE connection is considered lost.
                };

                status = ble_gap_update_params(event->connect.conn_handle, &params); /* Update connection parameters */
                if (status != 0)
                {
                    ESP_LOGE(TAG, "Failed to update connection parameters, error code: %d", status);
                }
            }
            else
            {
                ESP_LOGE(TAG, "Failed to find connection by handle, error code: %d", status);
            }
        }
        else /* Connection failed, restart advertising */
        {
            start_advertising();
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT: /* Disconnect event */
        ESP_LOGI(TAG, "Disconnected from peer; reason = %d", event->disconnect.reason);
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, 0));
        start_advertising(); /* Start advertising */
        break;

    case BLE_GAP_EVENT_CONN_UPDATE:                                                  /* Connection parameters update event */
        ESP_LOGI(TAG, "Connection updated; status = %d", event->conn_update.status); /* The central has updated the connection parameters. */
        status = ble_gap_conn_find(event->conn_update.conn_handle, &desc);           /* Check connection handle */
        if (status == 0)
        {
            print_conn_desc(&desc);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to find connection by handle, error code: %d", status);
        }
        break;

    default:
        break;
    }

    return status;
}

static void start_advertising(void)
{
    struct ble_hs_adv_fields adv_fields = {0};
    const char *name = ble_svc_gap_device_name();

    /* Set advertising flags */
    adv_fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    /* Set device name */
    adv_fields.name = (uint8_t *)name;
    adv_fields.name_len = strlen(name);
    adv_fields.name_is_complete = 1;

    /* Set device tx power */
    adv_fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    adv_fields.tx_pwr_lvl_is_present = 1;

    /* Set device appearance */
    adv_fields.appearance = BLE_GAP_APPEARANCE_GENERIC_TAG;
    adv_fields.appearance_is_present = 1;

    /* Set device LE role */
    adv_fields.le_role = BLE_GAP_ROLE_SLAVE;
    adv_fields.le_role_is_present = 1;

    /* Set advertiement fields */
    int status = ble_gap_adv_set_fields(&adv_fields);
    if (status == 0)
    {
        struct ble_hs_adv_fields rsp_fields = {0};

        /* Set URI */
        uint8_t esp_uri[] = {BLE_EDDYSTONE_URL_SCHEME_HTTPS, 'y', 'h', '.', 's', 'e'};
        rsp_fields.uri_len = sizeof(esp_uri);
        rsp_fields.uri = esp_uri;

        /* Set Manufacturer Data */
        // The first 2 bytes are the manufacturer ID (Espressif: 0x02E5)
        // Look at page 214 in https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf
        const uint8_t manufacturer[] = {0xE5, 0x02, 'Y', 'H', ' ', 'A', 'k', 'a', 'd', 'e', 'm', 'i', 'n'};
        rsp_fields.mfg_data_len = sizeof(manufacturer);
        rsp_fields.mfg_data = manufacturer;

        /* Set scan response fields */
        status = ble_gap_adv_rsp_set_fields(&rsp_fields);

        if (status == 0)
        {
            struct ble_gap_adv_params adv_params = {0};

            /* Set connetable and general discoverable mode */
            adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
            adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

            /* Set advertising interval */
            adv_params.itvl_min = BLE_GAP_ADV_ITVL_MS(INTERVAL);
            adv_params.itvl_max = adv_params.itvl_min + 1;

            /* Start advertising */
            status = ble_gap_adv_start(addr_type, NULL, BLE_HS_FOREVER, &adv_params, gap_event_handler, NULL);
            if (status == 0)
            {
                ESP_LOGI(TAG, "Advertising started!");
            }
            else
            {
                ESP_LOGE(TAG, "Failed to start advertising, error code: %d", status);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to set scan response data, error code: %d", status);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to set advertising data, error code: %d", status);
    }
}

bool gap_init(void)
{
    ble_svc_gap_init(); /* Initialize GAP service */

    int status = ble_svc_gap_device_name_set(DEVICE_NAME); /* Set GAP device name */

    if (status == 0)
    {
        /* Set GAP device appearance */
        status = ble_svc_gap_device_appearance_set(BLE_GAP_APPEARANCE_GENERIC_TAG);

        if (status != 0)
        {
            ESP_LOGE(TAG, "Failed to set device appearance, error code: %d", status);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to set device name to %s, error code: %d", DEVICE_NAME, status);
    }

    return (status == 0);
}

// on_stack_reset is called when host resets BLE stack due to errors
static void on_stack_reset(int reason)
{
    /* On reset, print reset reason to console */
    ESP_LOGI(TAG, "NimBLE stack reset, reset reason: %d", reason);
}

// on_stack_sync is called when host has synced with controller
static void on_stack_sync(void)
{
    int status = ble_hs_util_ensure_addr(0); /* Configure the device with a Bluetooth address */

    if (status == 0)
    {
        status = ble_hs_id_infer_auto(0, &addr_type); /* Figure out BT address to use while advertising */

        if (status == 0)
        {
            uint8_t addr[6] = {0};
            status = ble_hs_id_copy_addr(addr_type, addr, NULL); /* Copy device address to addr */
            if (status == 0)
            {
                char addr_str[18] = {0};
                sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
                ESP_LOGI(TAG, "Device Address: %s", addr_str);

                start_advertising(); /* Start advertising. */
            }
            else
            {
                ESP_LOGE(TAG, "failed to copy device address, error code: %d", status);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to infer address type, error code: %d", status);
        }
    }
    else
    {
        ESP_LOGE(TAG, "The device does not have any available bt address!");
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));

    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT));

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
            ESP_ERROR_CHECK(esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P20));

            if (!gap_init()) /* GAP service initialization */
            {
                ESP_LOGE(TAG, "Failed to initialize GAP service!");
            }
            else
            {
                /* NimBLE host configuration initialization */
                ble_hs_cfg.sync_cb = on_stack_sync;
                ble_hs_cfg.reset_cb = on_stack_reset;
                ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

                ESP_LOGI(TAG, "Start nimble host!");
                nimble_port_run(); /* This function will not return until nimble_port_stop() is called */
            }
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
