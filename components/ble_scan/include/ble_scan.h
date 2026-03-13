#ifndef BLE_SCAN_H
#define BLE_SCAN_H

#include "nimble/nimble_port.h"
#include "esp_err.h"
#include "host/ble_gap.h"
#include "freertos/event_groups.h"

#define DEVICE_FOUND_BIT BIT0

EventGroupHandle_t ble_scan_get_event_group(void);

esp_err_t ble_scan_start(const char *device);
esp_err_t ble_scan_stop(void);

// Função para guardar o endereço mac
void ble_scan_get_address(ble_addr_t *addr);

#endif /* BLE_SCAN_H */