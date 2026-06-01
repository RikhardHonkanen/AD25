#ifndef GAP_SVR_H
#define GAP_SVR_H

#include <stdint.h>
#include <stdbool.h>

#define DEVICE_NAME "SServer"

void gap_svr_init(void);

void gap_svr_adv_start(void);

bool gap_svr_is_connection_encrypted(uint16_t conn_handle);

#endif // GAP_SVR_H
