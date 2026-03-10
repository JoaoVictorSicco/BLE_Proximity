#include <stdio.h>
#include "ble_scan.h"
#include "driver_led.h"
#include "nvs_flash.h"


static const char *meu_dispositivo = "JV";

void app_main(void)
{
    // 1. Inicializa o nvs
    nvs_flash_init();

    // 2. Inicializa o led
    ble_led_config();

    // 3. Inicializa o BLE
    ble_scan_start(meu_dispositivo);

}
