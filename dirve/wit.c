#include "wit.h"




struct WitDevice{

    WIT_Data_t rec_data;
    uint8_t is_ready;
    uint8_t wit_dmaBuffer[WIT_DMA_SIZE];
    BusHandle_t bushandle;
};


static struct WitDevice g_my_wit_device;


WitHandle Wit_getInstace(void){


    return &g_my_wit_device;

}

void WitInit(WitHandle handle,BusHandle_t bus_handle){
    handle->is_ready = 1;
    handle->bushandle = bus_handle;
    bsp_wit_usart_receive_dma(handle->wit_dmaBuffer);
}
void WIT_ParseData(WitHandle handle){

    if(handle == NULL||!handle->is_ready){
        return;
    }
     uint8_t checkSum, packCnt = 0;
    uint8_t *buf = handle->wit_dmaBuffer;

    while (packCnt < 3) // 32/11 ˜ 3 ??
    {
        checkSum = 0;
        for (int i = packCnt*11; i < (packCnt+1)*11-1; i++)
            checkSum += buf[i];

        if (buf[packCnt*11] == 0x55 && checkSum == buf[packCnt*11+10])
        {
            uint8_t type = buf[packCnt*11+1];

            if (type == 0x51)
            {
                handle->rec_data.ax = (int16_t)((buf[packCnt*11+3]<<8) | buf[packCnt*11+2]);
                handle->rec_data.ay = (int16_t)((buf[packCnt*11+5]<<8) | buf[packCnt*11+4]);
                handle->rec_data.az = (int16_t)((buf[packCnt*11+7]<<8) | buf[packCnt*11+6]);
                handle->rec_data.temperature = (int16_t)((buf[packCnt*11+9]<<8) | buf[packCnt*11+8]) / 100.0f;
            }
            else if (type == 0x52)
            {
                handle->rec_data.gx = (int16_t)((buf[packCnt*11+3]<<8) | buf[packCnt*11+2]);
                handle->rec_data.gy = (int16_t)((buf[packCnt*11+5]<<8) | buf[packCnt*11+4]);
                handle->rec_data.gz = (int16_t)((buf[packCnt*11+7]<<8) | buf[packCnt*11+6]);
            }
            else if (type == 0x53)
            {
                handle->rec_data.roll  = (int16_t)((buf[packCnt*11+3]<<8) | buf[packCnt*11+2]) / 32768.0f * 180.0f;
                handle->rec_data.pitch = (int16_t)((buf[packCnt*11+5]<<8) | buf[packCnt*11+4]) / 32768.0f * 180.0f;
                handle->rec_data.yaw   = (int16_t)((buf[packCnt*11+7]<<8) | buf[packCnt*11+6]) / 32768.0f * 180.0f;
                handle->rec_data.version = (int16_t)((buf[packCnt*11+9]<<8) | buf[packCnt*11+8]);
            }
        }
        packCnt++;
    }

}
//从串口获得数据
void WitTask(WitHandle handle){

  BusHandle_t bus = handle->bushandle;
  WitInit(handle,bus);
  for(;;){
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    WIT_ParseData(handle);
    vTaskDelay(10);
    Bus_publish(bus,TOPIC_WIT,&handle->rec_data);
  }
}
