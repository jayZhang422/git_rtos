#include "bsp_usart.h"
//vofa上位机发送串口
void bsp_vofa_usart_transmit_byte(uint8_t data)
{
    HAL_UART_Transmit(&huart2, &data, 20, HAL_MAX_DELAY);
}

// 发送字符串
void bsp_vofa_usart_transmit_string(const char *str)
{
    if (str == NULL) return;
    HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

//wit串口
void bsp_wit_usart_trasmit(uint8_t data){
    HAL_UART_Transmit(&huart1,&data,4,HAL_MAX_DELAY);
}
//蓝牙串口
void bsp_ble_usart_trasmit(uint8_t data){
    HAL_UART_Transmit(&huart3,&data,sizeof(data),HAL_MAX_DELAY);
}
//蓝牙接收串口
void bsp_ble_usart_receive_dma(uint8_t *data){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3,data,10);
}
//wit发送串口（dma）
void bsp_wit_usart_trasmit_dma(uint8_t *data){
    HAL_UART_Transmit_DMA(&huart1,data,10);
}
//wit接收串口（dma）
void bsp_wit_usart_receive_dma(uint8_t *data){

    HAL_UART_Receive_DMA(&huart1,data,33);
}
