#ifndef GATT_SVR_H
#define GATT_SVR_H

#include "host/ble_gap.h"
#include "host/ble_gatt.h"

void gatt_svr_register_callback(struct ble_gatt_register_ctxt *ctxt, void *arg);

int gatt_svr_subscribe_callback(struct ble_gap_event *event);

void gatt_svr_init(void);

#endif // GATT_SVR_H