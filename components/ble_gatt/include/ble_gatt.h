#ifndef BLE_GATT_H
#define BLE_GATT_H

#include "esp_err.h"
#include "nimble/ble.h"

esp_err_t ble_gatt_connect(const ble_addr_t *addr);

#endif /* BLE_GATT_H */