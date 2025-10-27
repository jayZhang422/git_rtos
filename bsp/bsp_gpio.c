#include "bsp_gpio.h"

//拉高CS引脚
void bsp_cs_pull(){

HAL_GPIO_WritePin(ICM42688_CS_PORT,ICM42688_CS_PIN,GPIO_PIN_SET);

}
//拉低CS引脚
void bsp_cs_low(){
    
HAL_GPIO_WritePin(ICM42688_CS_PORT,ICM42688_CS_PIN,GPIO_PIN_RESET);

}

void bsp_ain1_run(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

}
void bsp_ain1_stop(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

}
void bsp_bin1_run(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

}
void bsp_bin1_stop(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

}
void bsp_ain2_run(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

}
void bsp_ain2_stop(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

}
void bsp_bin2_run(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

}
void bsp_bin2_stop(){


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

}
