#ifndef _BEEP_H_
#define _BEEP_H_
#include "stm32f10x.h"




//声明采用三目运算符
#define BUZZER(x) x?(GPIO_SetBits(GPIOB,GPIO_Pin_8)):(GPIO_ResetBits(GPIOB,GPIO_Pin_8))
#define FENGMi(x) x?(GPIO_SetBits(GPIOB,GPIO_Pin_8)):(GPIO_ResetBits(GPIOB,GPIO_Pin_8))
//置一的函数   GPIO_SetBits
//清零的函数   GPIO_ResetBits




void BEEP_config(void);//自己编写的函数(蜂鸣器的函数声明)


#endif


