#ifndef _VOFA_H_
#define _VOFA_H_



#include "bsp_usart.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#define Key1 GPIO_PIN_14
#define Key1_GPIO_Port GPIOC
#define Key2 GPIO_PIN_13
#define Key2_GPIO_Port GPIOC
#define Key3 GPIO_PIN_1
#define Key3_GPIO_Port GPIOA
void VOFA_SendMulti(const char *format, ...);
void VOFA_SendByte(uint8_t byte);
void VOFA_SendString(const char *str);


#endif
