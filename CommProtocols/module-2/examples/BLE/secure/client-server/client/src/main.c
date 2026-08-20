/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  The steps we need to take:
 *          1. Connect to a BLE peripheral
 *          2. Discover a service by UUID
 *          3. Discover a characteristic by UUID
 *          4. Discover its descriptors
 *          5. Enable notifications (by writing 0x0001 to CCCD)
 *          6. Receive notifications
 *
 * @version 0.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "bootloader_random.h"
#include "nimble/nimble_port.h"
#include "services/gap/ble_svc_gap.h"
#include "nimble/nimble_port_freertos.h"

#define BUFLEN 8
#define PASSKEY 123456
#define TAG DEVICE_NAME
#define DEVICE_NAME "CLIENT"

#define GATT_SVC_UUID 0xABC0 /* 16 Bit Service UUID */
#define GATT_CHR_UUID 0xABC1 /* 16 Bit Service Characteristic UUID */

static int gap_event(struct ble_gap_event *event, void *arg);

static uint8_t addr_type;
static uint16_t cnn_handle;
static ble_addr_t peer_addr;
static uint16_t chrval_handle;

// For random static address, 2 MSB bits of the first byte shall be 0b11.
// I.e. addr[5] shall be in the range of 0xC0 to 0xFF
static const uint8_t server_addr[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0xC0};
static const uint8_t client_addr[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0xC0};

// Initiates the GAP general discovery procedure.
static void scan(void)
{
    cnn_handle = BLE_HS_CONN_HANDLE_NONE;

    struct ble_gap_disc_params disc_params = {0};

    disc_params.passive = 1;           /* Perform a passive scan. */
    disc_params.filter_duplicates = 1; /* Avoid processing repeated advertisements from the same device. */

    int status = ble_gap_disc(addr_type, BLE_HS_FOREVER, &disc_params, gap_event, NULL);

    if (status != 0)
    {
        ESP_LOGE(TAG, "Error initiating GAP discovery procedure; rc=%d\n", status);
    }
}

static void connect(const struct ble_gap_disc_desc *disc)
{
    /* Scanning must be stopped before a connection can be initiated. */
    int status = ble_gap_disc_cancel();

    if (status == 0)
    {
        /* Try to connect the advertiser. 30 seconds timeout. It can be BLE_HS_FOREVER */
        status = ble_gap_connect(addr_type, &disc->addr, 30000, NULL, gap_event, NULL);

        if (status != 0)
        {
            char addr_str[18] = {0};
            sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X",
                    disc->addr.val[5], disc->addr.val[4], disc->addr.val[3],
                    disc->addr.val[2], disc->addr.val[1], disc->addr.val[0]);
            ESP_LOGE(TAG, "Error: Failed to connect to device; addr_type=%d addr=%s; status=%d\n", disc->addr.type, addr_str, status);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to cancel scan; status=%d\n", status);
    }
}

static int on_subscription(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg)
{
    if (error->status == 0)
    {
        cnn_handle = conn_handle;
    }
    else
    {
        cnn_handle = BLE_HS_CONN_HANDLE_NONE;
        ble_gap_terminate(conn_handle, BLE_ERR_CONN_TERM_LOCAL);
    }
    return error->status;
}

static int on_descriptor_discovery(uint16_t conn_handle, const struct ble_gatt_error *error, uint16_t chr_val_handle, const struct ble_gatt_dsc *dsc, void *)
{
    if ((error->status == 0) && (dsc != NULL))
    {
        if (0 == ble_uuid_cmp(&dsc->uuid.u, BLE_UUID16_DECLARE(BLE_GATT_DSC_CLT_CFG_UUID16)))
        {
            /* Subscribe to notifications for the characteristic.
             * A central enables notifications by writing two bytes (0x01 00) to the
             * characteristic's client-characteristic-configuration-descriptor (CCCD).
             * Notification: 0x01 00, Indication: 0x02 00 and Disable both: 0x00 00
             */
            uint8_t value[2] = {1, 0};
            assert(0 == ble_gattc_write_flat(conn_handle, dsc->handle, value, sizeof(value), on_subscription, NULL));
        }
    }
    else if (error->status != BLE_HS_EDONE)
    {
        ESP_LOGE(TAG, "Descriptor discovery failed: %d", error->status);
        ble_gap_terminate(conn_handle, BLE_ERR_CONN_TERM_LOCAL);
    }

    return 0;
}

static int on_characteristic_discovery(uint16_t conn_handle, const struct ble_gatt_error *error, const struct ble_gatt_chr *chr, void *arg)
{
    if ((error->status == 0) && (chr != NULL))
    {
        chrval_handle = chr->val_handle;
    }
    else if (error->status != BLE_HS_EDONE)
    {
        ESP_LOGE(TAG, "Characteristic discovery error: %d", error->status);
        ble_gap_terminate(conn_handle, BLE_ERR_CONN_TERM_LOCAL);
    }

    return 0;
}

static int on_service_discovery(uint16_t conn_handle, const struct ble_gatt_error *error, const struct ble_gatt_svc *service, void *arg)
{
    if ((error->status == 0) && (service != NULL))
    {
        assert(0 == ble_gattc_disc_chrs_by_uuid(conn_handle, service->start_handle, service->end_handle,
                                                BLE_UUID16_DECLARE(GATT_CHR_UUID), on_characteristic_discovery, NULL));
    }
    else if (error->status != BLE_HS_EDONE)
    {
        ESP_LOGE(TAG, "Service discovery failed; status=%d\n", error->status);
        ble_gap_terminate(conn_handle, BLE_ERR_CONN_TERM_LOCAL);
    }

    return 0;
}

static int gap_event(struct ble_gap_event *event, void *)
{
    int status = 0;
    struct ble_gap_conn_desc desc;
    struct ble_hs_adv_fields fields;

    switch (event->type)
    {
    case BLE_GAP_EVENT_DISC:
        if (0 == ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data))
        {
            /* Check if device is already not connected */
            if (0 != memcmp(peer_addr.val, event->disc.addr.val, sizeof(event->disc.addr.val)))
            {
                /* Check if the advertising event indicates a connectable advertisement */
                if ((event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_ADV_IND) || (event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_DIR_IND))
                {
                    if (0 == memcmp(event->disc.addr.val, server_addr, sizeof(server_addr)))
                    {
                        /* The device has to advertise support for the service. */
                        for (int i = 0; i < fields.num_uuids16; i++)
                        {
                            if (ble_uuid_u16(&fields.uuids16[i].u) == GATT_SVC_UUID)
                            {
                                /* Try to connect to the advertiser. */
                                connect(&event->disc);
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;

    case BLE_GAP_EVENT_CONNECT: /* A new connection was established or a connection attempt failed. */
        if (event->connect.status == 0)
        {
            assert(0 == ble_gap_conn_find(event->connect.conn_handle, &desc));

            assert(0 == ble_gap_security_initiate(event->subscribe.conn_handle)); /* Request connection encryption */

            memcpy(peer_addr.val, desc.peer_id_addr.val, sizeof(desc.peer_id_addr.val));

            assert(0 == ble_gattc_disc_svc_by_uuid(event->connect.conn_handle, BLE_UUID16_DECLARE(GATT_SVC_UUID), on_service_discovery, NULL));
        }
        else
        {
            /* Connection attempt failed; resume scanning. */
            ESP_LOGE(TAG, "Error: Connection failed; status=%d\n", event->connect.status);
            scan();
        }

        break;

    case BLE_GAP_EVENT_DISCONNECT:
        memset(peer_addr.val, 0, sizeof(peer_addr.val)); /* Forget about the peer */
        scan();                                          /* Resume scanning */

        break;

    case BLE_GAP_EVENT_ENC_CHANGE: /* Encryption change event: Encryption has been enabled or disabled for this connection. */
        if (event->enc_change.status == 0)
        {
            assert(0 == ble_gattc_disc_all_dscs(event->connect.conn_handle, chrval_handle, chrval_handle + 1, on_descriptor_discovery, NULL));
        }
        else
        {
            ESP_LOGE(TAG, "connection encryption failed, status: %d", event->enc_change.status);
        }
        break;

    case BLE_GAP_EVENT_NOTIFY_RX:
    {
        char buffer[OS_MBUF_PKTLEN(event->notify_rx.om)];
        memset(buffer, 0, sizeof(buffer));

        // Attribute data is in event->notify_rx.om.
        assert(0 == os_mbuf_copydata(event->notify_rx.om, 0, sizeof(buffer), buffer));

        printf("Received: %.*s\n", sizeof(buffer), buffer);
    }
    break;

    default:
        break;
    }

    return status;
}

static void on_reset(int reason)
{
    ESP_LOGE(TAG, "Resetting state; reason=%d\n", reason);
}

static void on_sync(void)
{
    assert(0 == ble_hs_id_set_rnd(client_addr)); // Set random static address; BLE_ADDR_RANDOM

    /* Make sure we have proper identity address set (public preferred) */
    assert(0 == ble_hs_util_ensure_addr(0));

    /* Figure out address to use while advertising */
    assert(0 == ble_hs_id_infer_auto(0, &addr_type));

    uint8_t addr[sizeof(server_addr)] = {0};
    assert(0 == ble_hs_id_copy_addr(addr_type, addr, NULL));

    printf("BLE Device Address: %02X:%02X:%02X:%02X:%02X:%02X\n", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);

    /* Begin scanning for a peripheral to connect to. */
    scan();
}

static void task(void *pvParameters)
{
    (void)pvParameters;

    bootloader_random_enable();
    srand(esp_random());
    bootloader_random_disable();

    uint8_t buffer[BUFLEN];

    while (1)
    {
        if (cnn_handle == BLE_HS_CONN_HANDLE_NONE)
        {
            vTaskDelay(5);
            continue;
        }

        for (int i = 0; i < BUFLEN; i++)
        {
            buffer[i] = 'a' + (rand() % 26);
        }

        printf("\n    Sent: %.*s\n", BUFLEN, buffer);

        /* Writing characteristics */
        if (0 != ble_gattc_write_flat(cnn_handle, chrval_handle, buffer, sizeof(buffer), NULL, NULL))
        {
            ESP_LOGE(TAG, "Error in writing characteristic");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    esp_err_t status = nvs_flash_init();
    if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);

    ESP_ERROR_CHECK(nimble_port_init());

    /* Configure the host. */
    ble_hs_cfg.sync_cb = on_sync;
    ble_hs_cfg.reset_cb = on_reset;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    /* Security manager configuration */
    ble_hs_cfg.sm_sc = 1;      // Secure Connections
    ble_hs_cfg.sm_mitm = 1;    // MITM protection = required for passkey
    ble_hs_cfg.sm_bonding = 1; // Enable bonding
    ble_hs_cfg.sm_io_cap = BLE_HS_IO_KEYBOARD_ONLY;
    ble_hs_cfg.sm_our_key_dist |= BLE_SM_PAIR_KEY_DIST_ENC;
    ble_hs_cfg.sm_their_key_dist |= BLE_SM_PAIR_KEY_DIST_ENC;

    assert(0 == ble_sm_configure_static_passkey(PASSKEY, true));

    /* Set the default device name. */
    assert(0 == ble_svc_gap_device_name_set(DEVICE_NAME));

    assert(pdTRUE == xTaskCreate(task, "task", 4096, NULL, 8, NULL));

    nimble_port_run(); /* This function will return only when nimble_port_stop() is executed */
    nimble_port_freertos_deinit();
}
