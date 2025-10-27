/*
@brief:BSP库的封装
@writer：zjw
@data：2025-10-6
@Programming language：C
*/

#ifndef _BSP_H
#define _BSP_H

#define stm32
#ifdef __cplusplus
extern "C" {
#endif
#include "bsp_gpio.h"
#include "bsp_i2c.h"
#include "bsp_spi.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

	#ifdef __cplusplus
}
#endif
#endif
