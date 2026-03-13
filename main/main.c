#include <stdio.h>
#include "ble_scan.h"
#include "driver_led.h"
#include "nimble/ble.h"
#include "nvs_flash.h"
#include "ble_gatt.h"
#include "freertos/FreeRTOS.h"


static const char *meu_dispositivo = "JV";
ble_addr_t addr;

void app_main(void)
{
    // 1. Inicializa o nvs
    nvs_flash_init();

    // 2. Inicializa o led
    ble_led_config();

    // 3. Inicializa o BLE
    ble_scan_start(meu_dispositivo);

    xEventGroupWaitBits(ble_scan_get_event_group(), DEVICE_FOUND_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
    ble_scan_stop();

    // 4. Obter o endereço
    ble_scan_get_address(&addr);
    
    // 5. Conectar o BLE
    ble_gatt_connect(&addr);

    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
