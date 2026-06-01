#include "gap_svr.h"
#include "gatt_svr.h"
#include "driver/gpio.h"
#include "services/gatt/ble_svc_gatt.h"

#define TAG "GATT"

static int led_chr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);

/* Automation IO service */
static uint16_t led_chr_val_handle;
static const ble_uuid16_t auto_io_svc_uuid = BLE_UUID16_INIT(0x1815); // Automation IO Service UUID
static const ble_uuid128_t led_chr_uuid = BLE_UUID128_INIT(0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
                                                           0xDE, 0xEF, 0x12, 0x12, 0x25, 0x15, 0x00, 0x00);

static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    /* Automation IO service */
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &auto_io_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){
            /* LED characteristic */
            {
                .uuid = &led_chr_uuid.u,
                .access_cb = led_chr_access,
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_READ_ENC | BLE_GATT_CHR_F_WRITE_ENC | BLE_GATT_CHR_F_INDICATE,
                .val_handle = &led_chr_val_handle,
            },
            {
                0, /* No more characteristics */
            },
        },
    },
    {
        0, /* No more services. */
    },
};

static int led_chr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *)
{
    int status = 0;

    if (attr_handle != led_chr_val_handle) /* Verify attribute handle */
    {
        ESP_LOGE(TAG, "Unexpected access operation to led characteristic, opcode: %d", ctxt->op);
        status = BLE_ATT_ERR_UNLIKELY;
    }
    else
    {
        switch (ctxt->op)
        {
        case BLE_GATT_ACCESS_OP_WRITE_CHR:                                                                 /* Write characteristic event */
            if ((conn_handle != BLE_HS_CONN_HANDLE_NONE) && !gap_svr_is_connection_encrypted(conn_handle)) /* Check connection and security status */
            {
                ESP_LOGE(TAG, "characteristic write; connection not encrypted!");
                status = BLE_ATT_ERR_INSUFFICIENT_AUTHEN;
            }
            else if (ctxt->om->om_len == 1) /* Verify access buffer length */
            {
                if (ctxt->om->om_data[0] == 1)
                {
                    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, 1));
                    ESP_LOGI(TAG, "led turned on!");
                }
                else
                {
                    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_4, 0));
                    ESP_LOGI(TAG, "led turned off!");
                }

                if (0 != ble_gatts_indicate(conn_handle, attr_handle))
                {
                    ESP_LOGE(TAG, "Failed to send indication!");
                }
            }
            else
            {
                ESP_LOGE(TAG, "Unexpected access operation to led characteristic, opcode: %d", ctxt->op);
                status = BLE_ATT_ERR_UNLIKELY;
            }
            break;

        case BLE_GATT_ACCESS_OP_READ_CHR:                                                                  /* Read characteristic event */
            if ((conn_handle != BLE_HS_CONN_HANDLE_NONE) && !gap_svr_is_connection_encrypted(conn_handle)) /* Check connection and security status */
            {
                ESP_LOGE(TAG, "characteristic read; connection not encrypted!");
                status = BLE_ATT_ERR_INSUFFICIENT_AUTHEN;
            }
            else
            {
                uint8_t value = gpio_get_level(GPIO_NUM_4);
                if (0 != os_mbuf_append(ctxt->om, &value, sizeof(value)))
                {
                    status = BLE_ATT_ERR_INSUFFICIENT_RES;
                }
            }
            break;

        default: /* Unknown event */
            ESP_LOGE(TAG, "Unexpected access operation to led characteristic, opcode: %d", ctxt->op);
            status = BLE_ATT_ERR_UNLIKELY;
            break;
        }
    }

    return status;
}

void gatt_svr_register_callback(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN] = {0};

    /* Handle GATT attributes register events */
    switch (ctxt->op)
    {
    case BLE_GATT_REGISTER_OP_SVC: /* Service register event */
        ESP_LOGD(TAG, "registered service %s with handle=%d", ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf), ctxt->svc.handle);
        break;

    case BLE_GATT_REGISTER_OP_CHR: /* Characteristic register event */
        ESP_LOGD(TAG, "registering characteristic %s with def_handle=%d val_handle=%d",
                 ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf), ctxt->chr.def_handle, ctxt->chr.val_handle);
        break;

    case BLE_GATT_REGISTER_OP_DSC: /* Descriptor register event */
        ESP_LOGD(TAG, "registering descriptor %s with handle=%d", ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf), ctxt->dsc.handle);
        break;

    default: /* Unknown event */
        assert(0);
        break;
    }
}

// GATT server subscribe event callback
int gatt_svr_subscribe_callback(struct ble_gap_event *event)
{
    int status = 0;

    /* Check attribute handle */
    if (event->subscribe.attr_handle == led_chr_val_handle)
    {
        /* Check security status */
        if (!gap_svr_is_connection_encrypted(event->subscribe.conn_handle))
        {
            ESP_LOGE(TAG, "Failed to subscribe, connection not encrypted!");
            status = BLE_ATT_ERR_INSUFFICIENT_AUTHEN;
        }
    }

    return status;
}

void gatt_svr_init(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT));

    ble_svc_gatt_init(); /* GATT service initialization */

    assert(0 == ble_gatts_count_cfg(gatt_svr_svcs)); /* Update GATT services counter */

    assert(0 == ble_gatts_add_svcs(gatt_svr_svcs)); /* Add GATT services */
}
