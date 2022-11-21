#ifndef  _TIME6_H_
#define  _TIME6_H_
#include "stm32f10x.h"


void TIM6_Config (u16 prescaler,u16 period);
void NVIC6_Config (void);

void TIM6_Seconds(void);
void TIM6_Seconds_2 (void);


#endif



