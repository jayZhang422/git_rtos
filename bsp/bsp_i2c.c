#include "bsp_i2c.h"

//OLED的命令发送
void bsp_oled_transmit_cmd(uint8_t dat,uint16_t size){

    HAL_I2C_Mem_Write(&OLED_COM_PORT,OLED_ADDR,OLED_CMD,I2C_MEMADD_SIZE_8BIT,&dat,size,100);
}
//OLED的数据发送
void bsp_oled_transmit_data(uint8_t* dat, uint16_t size){
   
    HAL_I2C_Mem_Write(&OLED_COM_PORT, OLED_ADDR, OLED_DATA, I2C_MEMADD_SIZE_8BIT, dat, size, 100);
}