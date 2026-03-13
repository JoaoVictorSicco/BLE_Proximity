#include <stdio.h>
#include "ble_gatt.h"
#include "esp_err.h"
#include "host/ble_hs.h"        // Header principal do Host Nimble
#include "host/ble_gap.h"       // Para ble_gap_event
#include "host/ble_gatt.h"      // Para ble_gatt_error, ble_gatt_svc, ble_gattc_disc_all_svcs
#include "os/os_mbuf.h"         // Para os_mbuf_copydata

/* UUID 0x2A19 = Battery Level (característica do Battery Service) */
#define BLE_UUID_BATTERY_LEVEL  0x2A19

static int on_read(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg){
    uint8_t battery_level;
    if(error->status != 0){
        if(error->status != 0){
            printf("On read FAIL, status: 0x%02x\n", (unsigned)error->status);
            return  1;
        }
        return  1;
    }
    os_mbuf_copydata(attr->om, 0, 1, &battery_level);
    printf("Status Battery: %d\n", battery_level);
    printf("Status Battery: %d (0x%02x)\n", battery_level, (unsigned)battery_level);
    return 0;
}

static int on_chr_disc(uint16_t conn_handle, const struct ble_gatt_error *error, const struct ble_gatt_chr *chr, void *arg){
    if (error->status == BLE_HS_EDONE) {
        return 0;  /* fim da descoberta de características */
    }
    if (error->status != 0) {
        printf("On chr disc FAIL: %d\n", error->status);
        return 1;
    }
    if (chr == NULL) {
        return 0;
    }
    /* Só nos interessa a característica Battery Level */
    if (ble_uuid_cmp(&chr->uuid, BLE_UUID16_DECLARE(BLE_UUID_BATTERY_LEVEL)) == 0) {
        ble_gattc_read(conn_handle, chr->val_handle, on_read, NULL);
    }
    return 0;
}

static int on_service_disc(uint16_t conn_handle, const struct ble_gatt_error *error, const struct ble_gatt_svc *service, void *arg){

    if(error->status == BLE_HS_EDONE){
        printf("Service discovery complete\n");
        return 0;
    }
    if(error->status != 0){
        printf("On service disc STATUS FAIL: %d\n", error->status);
        return 1;
    }
    
    if(service != NULL){
        printf("Service UUID: 0x%04x\n", ble_uuid_u16(&service->uuid));
    }

    if (ble_uuid_cmp(&service->uuid, BLE_UUID16_DECLARE(0x180F)) == 0) {
        // ble_gattc_disc_all_chrs(conn_handle, service->start_handle, service->end_handle,
        //                        on_chr_disc, NULL);
        ble_gattc_disc_chrs_by_uuid(conn_handle, service->start_handle, 
            service->end_handle, BLE_UUID16_DECLARE(BLE_UUID_BATTERY_LEVEL), on_chr_disc, NULL);

    }
    return 0;
}

static int on_connect(struct ble_gap_event *event, void *arg){
    
    switch(event->type){
        case BLE_GAP_EVENT_CONNECT: {
            if(event->connect.status == 0){
                printf("BLE_GAP_EVENT_CONNECT SUCESS\n");
                ble_gattc_disc_all_svcs(event->connect.conn_handle, on_service_disc, NULL);
            }
            else {
                printf("BLE_GAP_EVENT_CONNECT FAIL\n");
            } 
            break;
        }
    }
    return 0;
}

esp_err_t ble_gatt_connect(const ble_addr_t *addr){

    int ble_gap_return = ble_gap_connect(BLE_OWN_ADDR_PUBLIC, addr, BLE_HS_FOREVER, NULL, on_connect, NULL);
    if(ble_gap_return != 0) 
    {
        printf("ble_gap_connnect fail: %d\n", ble_gap_return);
        return ESP_FAIL;
    }
    return ESP_OK;
}