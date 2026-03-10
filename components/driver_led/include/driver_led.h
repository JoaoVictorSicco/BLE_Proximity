#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#include "esp_err.h"


#define LED_STATUS (21)
#define BLINK_DELAY_MS 200 // Tempo de piscada rápido (200ms)

esp_err_t ble_led_config(void);
esp_err_t ble_led_on(void);
esp_err_t ble_led_off(void);
int ble_led_get_level(void);
esp_err_t ble_led_toggle(uint8_t blink_count, uint16_t delay_ms);

#endif /* DRIVER_LED_H */