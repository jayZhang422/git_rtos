/*
@brief:BSP库的封装
@writer：zjw
@data：2025-10-6
@Programming language：C
*/

#ifndef _BSP_I2C_H
#define _BSP_I2C_H


#define OLED_ADDR (0x3C << 1) 
#define OLED_CMD 0x00//写命令操作
#define OLED_DATA 0x40//写数据操作
#include "bsp.h"

#ifdef stm32
#include "i2c.h"
#define OLED_COM_PORT	hi2c1
#endif

void bsp_oled_transmit_cmd(uint8_t dat,uint16_t size);
void bsp_oled_transmit_data(uint8_t* dat, uint16_t size);


#endif
