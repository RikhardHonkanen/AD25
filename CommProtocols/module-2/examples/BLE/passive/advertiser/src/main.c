/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of advertising as a non-connectable BLE device using NimBLE
 *        Install nRF Connect on your phone and scan
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
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "services/gap/ble_svc_gap.h"

#define TAG "Example"
#define DEVICE_NAME "ESP32"

// BLE defines appearance values to help remote devices understand what type of device they're interacting with.
// Look at page 30 of https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf
#define BLE_GAP_APPEARANCE_GENERIC_TAG 0x0200

static uint8_t addr_type;

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

    /* Set advertiement fields */
    int status = ble_gap_adv_set_fields(&adv_fields);
    if (status == 0)
    {
        struct ble_gap_adv_params adv_params = {0};

        /* Set non-connetable and general discoverable mode */
        adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
        adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

        /* Set advertising interval */
        adv_params.itvl_min = BLE_GAP_ADV_ITVL_MS(50);
        adv_params.itvl_max = adv_params.itvl_min + 1;

        /* Start advertising */
        status = ble_gap_adv_start(addr_type, NULL, BLE_HS_FOREVER, &adv_params, NULL, NULL);
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