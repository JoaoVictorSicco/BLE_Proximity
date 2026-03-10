#include <stdio.h>
#include "driver_led.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"

esp_err_t ble_led_config(void){

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED_STATUS),
        .pull_down_en = 0,
        .pull_up_en = 0
    };

    esp_err_t gpio_ret = gpio_config(&io_conf);
    if (gpio_ret != ESP_OK) {
        printf("Failed to initialize GPIO: %d\n", gpio_ret);
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t ble_led_on(void){
    esp_err_t ret = gpio_set_level(LED_STATUS, 1);
    if (ret != ESP_OK) {
        printf("Failed to set level high: %d\n", ret);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t ble_led_off(void){
    esp_err_t ret = gpio_set_level(LED_STATUS, 0);
    if (ret != ESP_OK) {
        printf("Failed to set level low: %d\n", ret);
        return ESP_FAIL;
    }
    return ESP_OK;
}

int ble_led_get_level(void){
    return gpio_get_level(LED_STATUS);
}

esp_err_t ble_led_toggle(uint8_t blink_count, uint16_t delay_ms){
    ble_led_off();

    for(int i = 0; i < blink_count; i++){
        ble_led_on();
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        ble_led_off();
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    return ESP_OK;
}