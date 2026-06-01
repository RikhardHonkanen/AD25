#include "esp_bt.h"
#include "gap_svr.h"
#include "gatt_svr.h"
#include "esp_random.h"
#include "host/ble_gap.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"

#define TAG "GAP"
#define INTERVAL 500

static void start_advertising(void);

static uint8_t addr_type;
static uint8_t addr_val[BLE_DEV_ADDR_LEN] = {0};

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
            start_advertising();
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT: /* Disconnect event */
        ESP_LOGI(TAG, "disconnected from peer; reason=%d", event->disconnect.reason);
        start_advertising(); /* Restart advertising */
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
        start_advertising(); /* Restart advertising */
        break;

    case BLE_GAP_EVENT_SUBSCRIBE: /* Subscribe event */
        ESP_LOGI(TAG, "subscribe event; conn_handle=%d attr_handle=%d reason=%d prevn=%d curn=%d previ=%d curi=%d",
                 event->subscribe.conn_handle, event->subscribe.attr_handle, event->subscribe.reason, event->subscribe.prev_notify,
                 event->subscribe.cur_notify, event->subscribe.prev_indicate, event->subscribe.cur_indicate);

        if (BLE_ATT_ERR_INSUFFICIENT_AUTHEN == gatt_svr_subscribe_callback(event)) /* GATT subscribe event callback */
        {
            status = ble_gap_security_initiate(event->subscribe.conn_handle); /* Request connection encryption */
        }
        break;

    case BLE_GAP_EVENT_MTU: /* MTU update event */
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
            printf("Enter passkey %lu on the client side\n", pkey.passkey);
            assert(0 == ble_sm_inject_io(event->passkey.conn_handle, &pkey)); // Inject security manager IO
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

bool gap_svr_is_connection_encrypted(uint16_t conn_handle)
{
    struct ble_gap_conn_desc desc;
    assert(0 == ble_gap_conn_find(conn_handle, &desc)); /* Get connection descriptor */
    return (desc.sec_state.encrypted == 1);
}

void gap_svr_adv_start(void)
{
    ble_addr_t addr;
    assert(0 == ble_hs_id_gen_rnd(0, &addr)); /* Generate new non-resolvable private address */

    assert(0 == ble_hs_id_set_rnd(addr.val)); /* Set address */

    assert(0 == ble_hs_util_ensure_addr(1)); /* Make sure we have proper BT identity address set */

    assert(0 == ble_hs_id_infer_auto(0, &addr_type)); /* Figure out BT address to use while advertising */

    assert(0 == ble_hs_id_copy_addr(addr_type, addr_val, NULL)); /* Copy device address to addr_val */

    printf("BLE Device Address: %02X:%02X:%02X:%02X:%02X:%02X\n", addr.val[5], addr.val[4], addr.val[3], addr.val[2], addr.val[1], addr.val[0]);

    start_advertising(); /* Start advertising. */
}

void gap_svr_init(void)
{
    ble_svc_gap_init(); /* Call NimBLE GAP initialization API */

    assert(0 == ble_svc_gap_device_name_set(DEVICE_NAME)); /* Set GAP device name */
}