#ifndef _BLE_SERVER_H_
#define _BLE_SERVER_H_

#include "stddef.h"

int ble_server_init();
void ble_server_release();
int ble_msg_send(char *buffer, size_t len, size_t timeout);

#endif // _BLE_SERVER_H_
