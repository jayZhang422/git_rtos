#ifndef _BUZZER_H
#define _BUZZER_H
#include "stdint.h"
void play_music(void);
void non_Buzzer_Test(void *argument);
void Set_Buzzer_Frequency(uint16_t frequency);
void Delay(uint32_t time) ;

void Play_Melody(void);


#endif
