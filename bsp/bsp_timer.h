/*
@brief:BSP库的封装
@writer：zjw
@data：2025-10-9
@Programming language：C
*/

#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "bsp.h"
#ifdef stm32
#include "tim.h"
#endif
void bsp_timer_duty(float value,uint8_t channel);

#endif
