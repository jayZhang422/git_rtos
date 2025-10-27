#include "key.h"







struct KeyDevice{

    uint8_t is_ready;
    KeyState_t get_key_state;
    BusHandle_t bushandle;
};

static struct KeyDevice g_my_key_device;

KeyHandle_t Key_getInstance(void) {
    return &g_my_key_device;
}

void KeyInit(KeyHandle_t handle,BusHandle_t bus_handle) {
    handle->is_ready = 1;
    handle->get_key_state = key_null;
    handle->bushandle = bus_handle;
}

void KeyScan(KeyHandle_t handle) {
  
   if(handle == NULL||!handle->is_ready){
       return;
   }
   BusHandle_t bus = handle->bushandle;
   GPIO_PinState lastKeyState[3] = {GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET};
    int key_pressed_this_cycle = 0;
   for(;;){

    for(int i=0;i<3;i++){
       
        GPIO_PinState current;
        switch (i)
        {
        case 0: current = HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1); break;
        case 1: current = HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2); break;
        case 2: current = HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3); break;
        default: 
        break;
        }
        if(current == GPIO_PIN_RESET &&lastKeyState[i] == GPIO_PIN_SET)
        {

            vTaskDelay(75);
            switch (i)
        {
        case 0: current = HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1); break;
        case 1: current = HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2); break;
        case 2: current = HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3); break;
        default: 
        break;}
             if(current == GPIO_PIN_RESET &&lastKeyState[i] == GPIO_PIN_SET){
                handle->get_key_state =(KeyState_t)i;
                key_pressed_this_cycle = 1;
             }
        }
        
       lastKeyState[i] = current;
        }
         if(key_pressed_this_cycle ){
        Bus_publish(bus,TOPIC_KEY,&(handle->get_key_state));
         key_pressed_this_cycle = 0;
         }
        vTaskDelay(20);
    }
   }
void KeyTask(KeyHandle_t handle){

    BusHandle_t bus = handle->bushandle;
    KeyInit(handle,bus);
    
        KeyScan(handle);
       
        
    
   

}
