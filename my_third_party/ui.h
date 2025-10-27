#ifndef UI_H
#define UI_H

#include "oled.h"
#include "bus.h"
#define BIT_UI_UPDATE ( 1 << 0 )
extern EventGroupHandle_t uiEventGroup;


struct MyUiDevice;


typedef struct MyUiDevice* MyUiHandle_t;
typedef enum {
    UI_ICM,
    UI_WIT,
    UI_BLE,
}MyUiState_t;

void MyUiInit(MyUiHandle_t handle,OledHandle_t oledHandle,BusHandle_t busHandle);

MyUiHandle_t Ui_getInstace(void);
static void Ui_KeyCallback(Topic_t topic, void *data);
static void Ui_WitCallback(Topic_t topic, void *data);
static void Ui_BleCallback(Topic_t topic, void *data);
static void Ui_IcmCallback(Topic_t topic, void *data);
void Show_ICM_Data(MyUiHandle_t handle);
void Show_WIT_Data(MyUiHandle_t handle);
void Show_BLE_Data(MyUiHandle_t handle);
void UiUpdateTask(void *argument); 

#endif
