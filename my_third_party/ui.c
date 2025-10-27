#include "ui.h"
#include "oledfont.h"
#include "buletooth.h"
#include "icm42688_driver.h"
#include "wit.h"
#include "key.h"
#include "string.h"
#include "MahonyAHRS.h"
EventGroupHandle_t uiEventGroup;
struct MyUiDevice{
    OledHandle_t oled_handle;
    uint8_t is_ready;
    MyUiState_t current_state;
    MyUiState_t previous_state;
    BusHandle_t Bus_handle;
    RxPID ble_data;
    WIT_Data_t rec_data;
    FLOAT_ANGLE att_angle;
};
static struct MyUiDevice g_my_uiDevice;

MyUiHandle_t Ui_getInstace(void){
    return &g_my_uiDevice;
}

void MyUiInit(MyUiHandle_t handle,OledHandle_t oledHandle,BusHandle_t busHandle){
    handle->is_ready = 1;
    handle->current_state = UI_BLE;
    uiEventGroup = xEventGroupCreate();
    handle->oled_handle = oledHandle;//将实际能够操作的句柄传入
    handle->Bus_handle = busHandle;
    Bus_subscribe(handle->Bus_handle,TOPIC_KEY,Ui_KeyCallback);
    Bus_subscribe(handle->Bus_handle,TOPIC_WIT,Ui_WitCallback);
    Bus_subscribe(handle->Bus_handle,TOPIC_BLE,Ui_BleCallback);
    Bus_subscribe(handle->Bus_handle,TOPIC_ICM,Ui_IcmCallback);
}
void Ui_KeyCallback(Topic_t topic, void *data){
    MyUiHandle_t handle = Ui_getInstace();
    if(!handle->is_ready) return;
    KeyState_t key = *((KeyState_t*)data);
    int state_changed = 0; // 标志位

    switch (key)
    {
    case  key1_pressed:
        handle->current_state = UI_ICM;
        state_changed = 1;
        break;
    case  key2_pressed:
        handle->current_state = UI_WIT;
        state_changed = 1;
        break;
    case  key3_pressed:
        handle->current_state = UI_BLE;
        state_changed = 1;
        break;
    default:
        break;
    }

 
    if (state_changed && uiEventGroup != NULL) {
      
        xEventGroupSetBits(uiEventGroup, BIT_UI_UPDATE);
    }

}
void Ui_BleCallback(Topic_t topic, void *data){

    MyUiHandle_t handle = &g_my_uiDevice;
    if(!handle->is_ready) return;
    
    memcpy(&handle->ble_data, data, sizeof(RxPID));
    if (handle->current_state == UI_BLE && uiEventGroup != NULL) {
        xEventGroupSetBits(uiEventGroup, BIT_UI_UPDATE);
    }
}
static void Ui_WitCallback(Topic_t topic, void *data){

    MyUiHandle_t handle = Ui_getInstace();
    if(!handle->is_ready) return;
    memcpy(&handle->rec_data, data, sizeof(WIT_Data_t));
    if (handle->current_state == UI_WIT && uiEventGroup != NULL) {
        xEventGroupSetBits(uiEventGroup, BIT_UI_UPDATE);
    }
}
static void Ui_IcmCallback(Topic_t topic, void *data){

    MyUiHandle_t handle = Ui_getInstace();
    if(!handle->is_ready) return;
    memcpy(&handle->att_angle, data, sizeof(FLOAT_ANGLE));
    if (handle->current_state == UI_ICM && uiEventGroup != NULL) {
        xEventGroupSetBits(uiEventGroup, BIT_UI_UPDATE);
    }
}
void Show_ICM_Data(MyUiHandle_t handle){
    OledHandle_t oled = handle->oled_handle;
    if(!handle->is_ready) return;
    OLED_Fill(oled,0);
char buffer[20]; 

    OLED_ShowString(oled, 0, 0, "[Key2: ICM Data]", 12);
    
    sprintf(buffer, "Pitch: %.2f", handle->att_angle.pit);
    OLED_ShowString(oled, 0, 16, buffer, 12);
    
    sprintf(buffer, "Roll:  %.2f", handle->att_angle.rol);
    OLED_ShowString(oled, 0, 32, buffer, 12);
    
    sprintf(buffer, "Yaw:   %.2f", handle->att_angle.yaw);
    OLED_ShowString(oled, 0, 48, buffer, 12);
    
}
void Show_WIT_Data(MyUiHandle_t handle){
 OledHandle_t oled = handle->oled_handle;
    if(!handle->is_ready) return;

    char buffer[20]; 

    OLED_ShowString(oled, 0, 0, "[Key1: WIT Data]", 12);
    
    sprintf(buffer, "Pitch: %.2f", handle->rec_data.pitch);
    OLED_ShowString(oled, 0, 16, buffer, 12);
    
    sprintf(buffer, "Roll:  %.2f", handle->rec_data.roll);
    OLED_ShowString(oled, 0, 32, buffer, 12);
    
    sprintf(buffer, "Yaw:   %.2f", handle->rec_data.yaw);
    OLED_ShowString(oled, 0, 48, buffer, 12);
  
}
void Show_BLE_Data(MyUiHandle_t handle){
 OledHandle_t oled = handle->oled_handle;
    if(!handle->is_ready) return;
OLED_ShowString(oled, 0, 0, "[Key3: BLE PID]", 12);
    char buffer[20];
    sprintf(buffer, "P: %.2f", handle->ble_data.p_val);
    OLED_ShowString(oled, 0, 16, buffer, 12);
    
    sprintf(buffer, "I: %.2f", handle->ble_data.i_val);
    OLED_ShowString(oled, 0, 32, buffer, 12);
    
    sprintf(buffer, "D: %.2f", handle->ble_data.d_val);
    OLED_ShowString(oled, 0, 48, buffer, 12);

}
void UiUpdateTask(void *argument) {
    
    // 获取 UI 和 OLED 的句柄
    MyUiHandle_t handle = Ui_getInstace();
    OledHandle_t oled = handle->oled_handle; // 你在 MyUiInit 中已经保存了它
    
    if (oled == NULL) {
        vTaskDelete(NULL); // 句柄无效，删除任务
    }

    // 任务主循环
    for(;;) {
        // 1. 等待信号
        //    它会一直阻塞，直到任何回调函数设置了 BIT_UI_UPDATE
        xEventGroupWaitBits(
            uiEventGroup,       // 要等待的事件组
            BIT_UI_UPDATE,      // 要等待的位
            pdTRUE,             // 退出时清除这个位 (这样下次就会等待新信号)
            pdFALSE,            // 不需要等待所有位 (OR)
            portMAX_DELAY       // 永久等待
        );

        // 2. 收到信号，执行所有慢速的绘图操作
        
        // 2a. 在这里统一清屏
        OLED_Fill(oled, 0); 

        // 2b. 根据当前状态，调用(飞快的)绘图函数
        switch(handle->current_state) {
            case UI_ICM:
                Show_ICM_Data(handle);
                break;
            case UI_WIT:
                Show_WIT_Data(handle);
                break;
            case UI_BLE:
                Show_BLE_Data(handle);
                break;
        }
        
        // 2c. 在这里统一刷新屏幕
        OLED_Refresh(oled); 
        
        
        vTaskDelay(50); 
    }
}

// 创建 UI 任务的函数
