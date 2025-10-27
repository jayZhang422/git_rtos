#ifndef WIT_H
#define WIT_H

#include "bsp.h"
#include "bus.h"
#define WIT_DMA_SIZE 32

struct WitDevice;

extern TaskHandle_t g_wit_task_handle;
typedef struct WitDevice* WitHandle;
typedef struct {
    float pitch;
    float roll;
    float yaw;
    float temperature;
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
    int16_t version;
} WIT_Data_t;

void WitInit(WitHandle handle,BusHandle_t bus_handle);

void WIT_ParseData(WitHandle handle);

WitHandle Wit_getInstace(void);

void WitTask(WitHandle handle);
#endif
