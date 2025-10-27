#include  "bsp_timer.h"



void bsp_timer_duty(float value,uint8_t channel){


__HAL_TIM_SET_COMPARE(&htim2, channel, value);


}
