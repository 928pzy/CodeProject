#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include "stm32f10x.h"


#define Delay_ms(x)   Delay_us(1000*x)



//初始化滴答时钟
void Systick_Config(void);

//用滴答时钟实现延时函数
void Delay_us (u32 time);











#endif


