#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include "stm32f10x.h"


#define Delay_ms(x)   Delay_us(1000*x)



//��ʼ���δ�ʱ��
void Systick_Config(void);

//�õδ�ʱ��ʵ����ʱ����
void Delay_us (u32 time);











#endif


