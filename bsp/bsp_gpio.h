/*
@brief:BSP库的封装
@writer：zjw
@data：2025-10-6
@Programming language：C
*/

#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "bsp.h"

#ifdef stm32
#include "gpio.h"
#define ICM42688_CS_PIN GPIO_PIN_4
#define ICM42688_CS_PORT GPIOA
#endif

void bsp_cs_pull(void);
void bsp_cs_low(void);
void bsp_ain1_run(void);
void bsp_ain1_stop(void);
void bsp_bin1_run(void);
void bsp_bin1_stop(void);
void bsp_ain2_run(void);
void bsp_ain2_stop(void);
void bsp_bin2_run(void);
void bsp_bin2_stop(void);


#endif
