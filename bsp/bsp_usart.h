/*
@brief:BSP库的UART封装
@writer：zjw
@data：2025-10-6
@Programming language：C
*/

#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "bsp.h"

#ifdef stm32
#include "usart.h"
#endif
void bsp_vofa_usart_transmit_byte(uint8_t data);
void bsp_vofa_usart_transmit_string(const char *str);
void bsp_wit_usart_trasmit(uint8_t data);
void bsp_ble_usart_trasmit(uint8_t data);
void bsp_wit_usart_trasmit_dma(uint8_t *data);
void bsp_wit_usart_receive_dma(uint8_t *data);
void bsp_ble_usart_receive_dma(uint8_t *data);
#endif
