#include <stdlib.h>
#include "esp_bt.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "host/ble_hs.h"
#include "driver/gpio.h"
#include "host/ble_gap.h"
#include "host/ble_gatt.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "nimble/nimble_port_freertos.h"

#define INTERVAL 500
#define TAG DEVICE_NAME
#define DEVICE_NAME "LEDServer"

static void advertise(void);
extern void ble_store_config_init(void); /* Library function declaration */
static int led_chr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);

/* Automation IO service */
static uint16_t led_chr_val_handle;
static const ble_uuid16_t auto_io_svc_uuid = BLE_UUID16_INIT(0x1815); // Automation IO Service UUID
static const ble_uuid128_t led_chr_uuid = BLE_UUID128_INIT(0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
                                                           0xDE, 0xEF, 0x12, 0x12, 0x25, 0x15, 0x00, 0x00);

static bool encrypted;
static uint8_t addr_type;
static uint8_t addr_val[BLE_DEV_ADDR_LEN] = {0};
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
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_READ_ENC | BLE_GATT_CHR_F_READ_AUTHEN |
                         BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_ENC | BLE_GATT_CHR_F_WRITE_AUTHEN,
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

static int gap_event_handler(struct ble_gap_event *event, void *)
{
    int status = 0;
    struct ble_gap_conn_desc desc;

    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT: /* Connect event */
        /* A new connection was established or a connection attempt failed. */
        ESP_LOGI(TAG, "connection %s; status=%d", event->connect.status == 0 ? "established" : "failed", event->connect.status);

        if (event->connect.status == 0) /* Connection succeeded */
        {
            /* Check connection handle */
            assert(0 == ble_gap_conn_find(event->connect.conn_handle, &desc));

            /* Try to update connection parameters */
            struct ble_gap_upd_params params = {
                .itvl_min = desc.conn_itvl,
                .itvl_max = desc.conn_itvl,
                .latency = 3,
                .supervision_timeout = desc.supervision_timeout,
            };
            assert(0 == ble_gap_update_params(event->connect.conn_handle, &params));
        }
        else /* Connection failed, restart advertising */
        {
            advertise();
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT: /* Disconnect event */
        ESP_LOGI(TAG, "disconnected from peer; reason=%d", event->disconnect.reason);
        advertise(); /* Restart advertising */
        break;

    case BLE_GAP_EVENT_CONN_UPDATE: /* The central has updated the connection parameters. */

        ESP_LOGI(TAG, "connection updated; status=%d", event->conn_update.status);

        assert(0 == ble_gap_conn_find(event->conn_update.conn_handle, &desc)); /* Get connection descriptor */

        ESP_LOGI(TAG, "connection handle: %d", desc.conn_handle); /* Connection handle */

        /* Local ID address */
        ESP_LOGI(TAG, "Device id address: type= %d, value = %02X:%02X:%02X:%02X:%02X:%02X", desc.our_id_addr.type,
                 desc.our_id_addr.val[5], desc.our_id_addr.val[4], desc.our_id_addr.val[3],
                 desc.our_id_addr.val[2], desc.our_id_addr.val[1], desc.our_id_addr.val[0]);

        /* Peer ID address */
        ESP_LOGI(TAG, "Peer id address: type= %d, value = %02X:%02X:%02X:%02X:%02X:%02X", desc.peer_id_addr.type,
                 desc.peer_id_addr.val[5], desc.peer_id_addr.val[4], desc.peer_id_addr.val[3],
                 desc.peer_id_addr.val[2], desc.peer_id_addr.val[1], desc.peer_id_addr.val[0]);

        /* Connection info */
        ESP_LOGI(TAG, "conn_itvl=%d, conn_latency=%d, supervision_timeout=%d, encrypted=%d, authenticated=%d, bonded=%d\n",
                 desc.conn_itvl, desc.conn_latency, desc.supervision_timeout, desc.sec_state.encrypted,
                 desc.sec_state.authenticated, desc.sec_state.bonded);
        break;

    case BLE_GAP_EVENT_ADV_COMPLETE: /* Advertising complete event */
        ESP_LOGI(TAG, "advertise complete; reason=%d", event->adv_complete.reason);
        advertise(); /* Restart advertising */
        break;

    case BLE_GAP_EVENT_MTU:
        /* Maximum Transmission Unit defines the maximum size of a single ATT (Attribute Protocol) payload,
         i.e., how much data can be sent in a single BLE GATT read/write/notify/indication operation. */
        ESP_LOGI(TAG, "MTU update event; conn_handle=%d cid=%d mtu=%d", event->mtu.conn_handle, event->mtu.channel_id, event->mtu.value);
        break;

    case BLE_GAP_EVENT_ENC_CHANGE: /* Encryption change event: Encryption has been enabled or disabled for this connection. */
        if (event->enc_change.status == 0)
        {
            ESP_LOGI(TAG, "connection encrypted!");
        }
        else
        {
            ESP_LOGE(TAG, "connection encryption failed, status: %d", event->enc_change.status);
        }

        assert(0 == ble_gap_conn_find(event->connect.conn_handle, &desc));
        encrypted = (desc.sec_state.encrypted == 1);

        break;

    case BLE_GAP_EVENT_REPEAT_PAIRING:                                            /* Repeat pairing event */
        assert(0 == ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc)); /* Get connection descriptor */
        assert(0 == ble_store_util_delete_peer(&desc.peer_id_addr));              /* Delete the old bond */

        ESP_LOGI(TAG, "Repairing...");
        status = BLE_GAP_REPEAT_PAIRING_RETRY; // Return BLE_GAP_REPEAT_PAIRING_RETRY to indicate that
                                               // the host should continue with pairing operation
        break;

    case BLE_GAP_EVENT_PASSKEY_ACTION:                         /* Passkey action event */
        if (event->passkey.params.action == BLE_SM_IOACT_DISP) /* Display action */
        {
            /* Generate passkey */
            struct ble_sm_io pkey = {0};
            pkey.action = event->passkey.params.action;
            pkey.passkey = 100000 + esp_random() % 900000;
            printf("On the client, enter passkey: %03lu %03lu\n", pkey.passkey / 1000, pkey.passkey % 1000);
            assert(0 == ble_sm_inject_io(event->passkey.conn_handle, &pkey)); // Inject security manager IO
        }
        break;

    default:
        break;
    }

    return status;
}

static void advertise(void)
{
    encrypted = false;
    struct ble_hs_adv_fields adv_fields = {0};

    /* Set advertising flags */
    adv_fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    /* Set device name */
    const char *name = ble_svc_gap_device_name();
    adv_fields.name = (uint8_t *)name;
    adv_fields.name_len = strlen(name);
    adv_fields.name_is_complete = 1;

    /* Set device tx power */
    adv_fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    adv_fields.tx_pwr_lvl_is_present = 1;

    /* The Automation IO Service UUID */
    // Look at page 67 of https://www.bluetooth.com/wp-content/uploads/Files/Specification/Assigned_Numbers.pdf
    adv_fields.uuids16 = (ble_uuid16_t[]){BLE_UUID16_INIT(0x1815)};
    adv_fields.uuids16_is_complete = 1;
    adv_fields.num_uuids16 = 1;

    /* Set device LE role */
    adv_fields.le_role = BLE_GAP_ROLE_SLAVE;
    adv_fields.le_role_is_present = 1;

    /* Set advertiement fields */
    assert(0 == ble_gap_adv_set_fields(&adv_fields));

    struct ble_hs_adv_fields rsp_fields = {0};

    /* Set device address */
    rsp_fields.device_addr = addr_val;
    rsp_fields.device_addr_type = addr_type;
    rsp_fields.device_addr_is_present = 1;

    /* Set advertising interval */
    rsp_fields.adv_itvl = BLE_GAP_ADV_ITVL_MS(INTERVAL);
    rsp_fields.adv_itvl_is_present = 1;

    /* Set scan response fields */
    assert(0 == ble_gap_adv_rsp_set_fields(&rsp_fields));

    struct ble_gap_adv_params adv_params = {0};

    /* Set connetable and general discoverable mode */
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    /* Set advertising interval */
    adv_params.itvl_min = BLE_GAP_ADV_ITVL_MS(INTERVAL);
    adv_params.itvl_max = adv_params.itvl_min + 1;

    /* Start advertising */
    assert(0 == ble_gap_adv_start(addr_type, NULL, BLE_HS_FOREVER, &adv_params, gap_event_handler, NULL));
}

static void gap_adv_start(void)
{
    ble_addr_t addr;
    assert(0 == ble_hs_id_gen_rnd(1, &addr)); /* Generate new resolvable private address */

    assert(0 == ble_hs_id_set_rnd(addr.val)); /* Set address */

    assert(0 == ble_hs_util_ensure_addr(1)); /* Make sure we have proper BT identity address set */

    assert(0 == ble_hs_id_infer_auto(1, &addr_type)); /* Figure out BT address to use while advertising */

    assert(0 == ble_hs_id_copy_addr(addr_type, addr_val, NULL)); /* Copy device address to addr_val */

    printf("BLE Device Address: %02X:%02X:%02X:%02X:%02X:%02X\n", addr.val[5], addr.val[4], addr.val[3], addr.val[2], addr.val[1], addr.val[0]);

    advertise(); /* Start advertising. */
}

static void gap_init(void)
{
    ble_svc_gap_init(); /* Call NimBLE GAP initialization API */

    assert(0 == ble_svc_gap_device_name_set(DEVICE_NAME)); /* Set GAP device name */
}

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
        case BLE_GATT_ACCESS_OP_WRITE_CHR:                              /* Write characteristic event */
            if ((conn_handle != BLE_HS_CONN_HANDLE_NONE) && !encrypted) /* Check connection and security status */
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
            }
            else
            {
                ESP_LOGE(TAG, "Unexpected access operation to led characteristic, opcode: %d", ctxt->op);
                status = BLE_ATT_ERR_UNLIKELY;
            }
            break;

        case BLE_GATT_ACCESS_OP_READ_CHR:                               /* Read characteristic event */
            if ((conn_handle != BLE_HS_CONN_HANDLE_NONE) && !encrypted) /* Check connection and security status */
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

static void gatt_svr_register_callback(struct ble_gatt_register_ctxt *ctxt, void *arg)
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

static void gatt_init(void)
{
    ble_svc_gatt_init(); /* GATT service initialization */

    assert(0 == ble_gatts_count_cfg(gatt_svr_svcs)); /* Update GATT services counter */

    assert(0 == ble_gatts_add_svcs(gatt_svr_svcs)); /* Add GATT services */
}

static void on_stack_reset(int reason)
{
    ESP_LOGI(TAG, "nimble stack reset, reset reason: %d", reason);
}

static void nimble_host_config_init(void)
{
    /* Set host callbacks */
    ble_hs_cfg.sync_cb = gap_adv_start;
    ble_hs_cfg.reset_cb = on_stack_reset;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_callback;

    /* Security manager configuration */
    ble_hs_cfg.sm_io_cap = BLE_HS_IO_DISPLAY_ONLY;
    ble_hs_cfg.sm_bonding = 1; // Enable bonding
    ble_hs_cfg.sm_mitm = 1;    // MITM protection = required for passkey
    ble_hs_cfg.sm_sc = 1;      // Secure Connections
    ble_hs_cfg.sm_our_key_dist |= BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID;
    ble_hs_cfg.sm_their_key_dist |= BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID;

    /* Store host configuration */
    ble_store_config_init();
}

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_4));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT_OUTPUT));

    srand(esp_random()); /* Random generator initialization */

    /* NVS flash initialization */
    esp_err_t status = nvs_flash_init();
    if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);

    assert(0 == nimble_port_init());

    ESP_ERROR_CHECK(esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P20)); /* Set TX Power */

    gap_init();
    gatt_init();

    /* NimBLE host configuration initialization */
    nimble_host_config_init();

    nimble_port_run(); /* This function will return only when nimble_port_stop() is executed */
    nimble_port_freertos_deinit();
}