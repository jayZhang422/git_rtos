#include "bsp_spi.h"

//imu的数据发送
void bsp_imu_transmit(uint8_t *data){
    HAL_SPI_Transmit(&hspi1,data,2,HAL_MAX_DELAY);
}
//imu的数据发送和接收
void bsp_imu_RT(uint8_t *tx_data,uint8_t *rx_data){
    HAL_SPI_TransmitReceive(&hspi1,tx_data,rx_data,2,HAL_MAX_DELAY);
}
