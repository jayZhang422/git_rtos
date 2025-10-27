#ifndef _BULETOOTH_H_
#define _BULETOOTH_H_



#define RXBUFFERSIZE 32
#include "bsp.h"
#include <stdlib.h>
#include "bus.h"
struct BleDevice;

typedef struct {

    float p_val;
    float i_val;
    float d_val;

}RxPID;
typedef struct BleDevice* BleHandle_t;

extern TaskHandle_t g_ble_task_handle;

void BleInit(BleHandle_t handle,BusHandle_t bus_handle);

void BleReciveData(BleHandle_t handle , uint8_t* rxData , uint16_t Size);

BleHandle_t Ble_getInstace(void);
void ble_start_dma(BleHandle_t handle);
void ble_task(BleHandle_t handle);






#endif
