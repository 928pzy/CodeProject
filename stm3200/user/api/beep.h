#ifndef _BEEP_H_
#define _BEEP_H_
#include "stm32f10x.h"




//����������Ŀ�����
#define BUZZER(x) x?(GPIO_SetBits(GPIOB,GPIO_Pin_8)):(GPIO_ResetBits(GPIOB,GPIO_Pin_8))
#define FENGMi(x) x?(GPIO_SetBits(GPIOB,GPIO_Pin_8)):(GPIO_ResetBits(GPIOB,GPIO_Pin_8))
//��һ�ĺ���   GPIO_SetBits
//����ĺ���   GPIO_ResetBits




void BEEP_config(void);//�Լ���д�ĺ���(�������ĺ�������)


#endif


