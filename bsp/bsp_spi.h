/*
@brief:BSP库的封装
@writer：zjw
@data：2025-10-6
@Programming language：C
*/

#ifndef _BSP_SPI_H
#define _BSP_SPI_H

#include "bsp.h"
#ifdef stm32
#include "spi.h"
#endif

void bsp_imu_transmit(uint8_t *data);
void bsp_imu_RT(uint8_t *tx_data,uint8_t *rx_data);

#endif
