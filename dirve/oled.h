#ifndef OLED_H
#define OLED_H


#include "bsp.h"

#define SSD1315
struct OledDevice;

typedef struct OledDevice* OledHandle_t;


//初始化
void Oled_Init(OledHandle_t handle);


//清屏
void OLED_Fill(OledHandle_t handle,uint8_t ucData);
//显示char类型字符
void OLED_ShowChar(OledHandle_t handle,uint8_t x,uint8_t y,uint8_t chr,uint8_t size1);
//显示字符串
void OLED_ShowString(OledHandle_t handle,uint8_t x,uint8_t y,char *chr,uint8_t size1);
//刷新
void OLED_Refresh(OledHandle_t handle);
//反显
void OLED_ColorTurn(OledHandle_t handle,uint8_t i);

void OLED_DrawPoint(OledHandle_t handle,uint8_t x,uint8_t y);

void OLED_ClearPoint(OledHandle_t handle,uint8_t x,uint8_t y);

OledHandle_t Oled_getInstance(void);

void OLED_ReverseArea(OledHandle_t handle,int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

void OLED_Draw_Picture(OledHandle_t handle,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t mode, unsigned const char p[]);   

#endif
