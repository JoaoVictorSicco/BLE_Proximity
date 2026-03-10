#ifndef BLE_SCAN_H
#define BLE_SCAN_H

#include "nimble/nimble_port.h"
#include "esp_err.h"

esp_err_t ble_scan_start(const char *device);
esp_err_t ble_scan_stop(void);

#endif /* BLE_SCAN_H */