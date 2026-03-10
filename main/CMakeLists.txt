idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES ble_gatt ble_scan driver_led nvs_flash)
