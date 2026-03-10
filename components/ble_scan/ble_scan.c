#include <stdio.h>
#include "ble_scan.h"
#include <string.h>
#include "host/ble_gap.h"
#include "driver_led.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h" 
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"

static const char *my_device;

static int ble_scan_callback(struct ble_gap_event *event, void *arg){
    
    switch(event->type){
        case BLE_GAP_EVENT_DISC: {
            printf("Dispositivo detectado, RSSI: %d\n", event->disc.rssi);
            struct ble_hs_adv_fields fields;
            int parse_payload;
        
            // Faz o parse do pacote de advertising
            parse_payload = ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data);
            if(parse_payload != 0){
                printf("Erro ao processar o pacote adversiting: %d\n", parse_payload);
                return 0;
            }
            if(fields.name != NULL){
                printf("Nome: %.*s\n", fields.name_len, fields.name);
            } else {
                printf("Sem nome\n");
            }            
        
            if(fields.name != NULL && my_device != NULL){
                printf("Nome: %.*s\n", fields.name_len, fields.name);
                if(!(strncmp((char *)fields.name, my_device, fields.name_len))){
                    printf("Dispositivo encontrado: %s\n", my_device);

                    if(event->disc.rssi > -60) ble_led_toggle(3, 150);                                  // Perto: acima de -60dB                           
                    else if(event->disc.rssi <= -60 && event->disc.rssi >= -80) ble_led_toggle(3, 500); // Médio: entre -60dB e -80dB
                    else if(event->disc.rssi < -80) ble_led_toggle(3, 1000);                             // Longe: abaixo de -80dB
                }
            }
            break;
        }
        case BLE_GAP_EVENT_DISC_COMPLETE:{
            break;
        }
    }
    return 0;
}

static void ble_host_task(void *param){
    nimble_port_run();
    nimble_port_freertos_deinit();
}

esp_err_t ble_scan_start(const char *device){
    if(device == NULL) return ESP_ERR_INVALID_ARG;
    my_device = device;

    // 1. Inicializa o stack NimBLE
    esp_err_t ret = nimble_port_init();
    if (ret != ESP_OK) {
        printf("Failed to initialize nimble_port: %d\n", ret);
        return ESP_FAIL;
    }

    // 2. Cria a task do FreeRTOS que roda o stack
    nimble_port_freertos_init(ble_host_task);

    // 3. Configurar o scan
    struct ble_gap_disc_params config_scan = {
        .filter_duplicates = 0,
        .passive           = 0,
        .itvl              = BLE_HCI_SCAN_ITVL_DEF,
        .window            = BLE_HCI_SCAN_WINDOW_DEF
    };

    // 4. Função que inicializa o scan
    int ble_disc = ble_gap_disc(BLE_OWN_ADDR_PUBLIC, BLE_HS_FOREVER, &config_scan, ble_scan_callback, NULL);
    if(ble_disc != 0){
        printf("Failed to start scan: %d\n", ble_disc);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t ble_scan_stop(void){

    int ble_stop = ble_gap_disc_cancel();
    if(ble_stop != 0){
        printf("Failed to stop scan: %d\n", ble_stop);
        return ESP_FAIL;
    }
    return ESP_OK;
}
