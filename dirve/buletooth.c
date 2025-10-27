#include "buletooth.h"
#include "usart.h"
#include "dma.h"
//存储接收数据的结构体
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
//私有变量
struct BleDevice
{
   
    uint8_t rxData[RXBUFFERSIZE];
    uint8_t is_ready;
    RxPID rx_pid;
    BusHandle_t bushandle;
    
};
void ble_start_dma(BleHandle_t handle){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3,handle->rxData,sizeof(handle->rxData));
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
}
//创建事例
static struct BleDevice g_my_ble_device;

//用函数的形式降低耦合度
BleHandle_t Ble_getInstace(void){


    return &g_my_ble_device;


}
//初始化接收，未初始化时，将不会进行后续任何操作

void BleInit(BleHandle_t handle,BusHandle_t bus_handle){

    handle->is_ready = 1;
     bsp_ble_usart_receive_dma(handle->rxData);
   __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
    handle->bushandle = bus_handle;

}
//真正接收函数
void BleReciveData(BleHandle_t handle, uint8_t *rxData, uint16_t Size){

        if(handle == NULL|| !handle->is_ready){
            //未传入任何句柄，或者未进行初始化，则直接返回

            return;

        }
       if(Size >= RXBUFFERSIZE) Size = RXBUFFERSIZE - 1;
    rxData[Size] = '\0';

    // 检查包头
    if(rxData[0] != 'K' || rxData[1] != 'K') return;

    int i = 2; // 从第三个字符开始
    char numStr[32];

    while(i < Size)
    {
        if(rxData[i] == 'P' || rxData[i] == 'p')
        {
            i++;
            int j = 0;
            while(i < Size && rxData[i] != 'I' && rxData[i] != 'i' && rxData[i] != 'D' && rxData[i] != 'd' && j < sizeof(numStr)-1)
                numStr[j++] = rxData[i++];
            numStr[j] = '\0';
            handle->rx_pid.p_val = strtod(numStr, NULL);
        }
        else if(rxData[i] == 'I' || rxData[i] == 'i')
        {
            i++;
            int j = 0;
            while(i < Size && rxData[i] != 'P' && rxData[i] != 'p' && rxData[i] != 'D' && rxData[i] != 'd' && j < sizeof(numStr)-1)
                numStr[j++] = rxData[i++];
            numStr[j] = '\0';
           handle->rx_pid.i_val = strtod(numStr, NULL);
        }
        else if(rxData[i] == 'D' || rxData[i] == 'd')
        {
            i++;
            int j = 0;
            while(i < Size && rxData[i] != 'P' && rxData[i] != 'p' && rxData[i] != 'I' && rxData[i] != 'i' && j < sizeof(numStr)-1)
                numStr[j++] = rxData[i++];
            numStr[j] = '\0';
           handle->rx_pid.d_val = strtod(numStr, NULL);
        }
        else
        {
            i++;
        }
    }
 


}
void ble_task(BleHandle_t handle){
    BusHandle_t bus = handle->bushandle;
   BleInit(handle,bus);
    for(;;){

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        BleReciveData(handle,handle->rxData,RXBUFFERSIZE);
        Bus_publish(bus,TOPIC_BLE,&(handle->rx_pid));
    }
}
