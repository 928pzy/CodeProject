/*
1������ʱ��Դ��Ϊ��ʼ��ϵͳʱ�ӡ�������֮����ر�ʱ�ӣ���ֹʱ��������֮��ʼ��ʱ��
*/


#include "systick.h"
#include "stm32f10x.h"

u32 systick_time = 0; //ȫ�ֱ���

#define  LOAD_1US      72
#define  LOAD_10US     720
#define  LOAD_100US    7200
#define  LOAD_1000US   72000

//**********************************************************************
/*
��������:Systick_Config
��������:��ʼ��
������������
��������ֵ����
*/
void Systick_Config(void)
{
	//����ʱ��Դ
	while (SysTick_Config(LOAD_1US) == 1);  //��LOAD_1US��Ϊ��ֵ������������ж���ֵ�Ƿ����2^24
	//�ر�ʱ��
	SysTick->CTRL &= ~(1<<0);				
}

//*******************************************************************************
/*
��������:Delay_us
��������:��ʱ����
������������
��������ֵ����
*/
void Delay_us (u32 time)
{
	systick_time = time;
	//��ʱ��
	SysTick->CTRL |= (1<<0);
	
	while (systick_time);
	
	//�ر�ʱ��
	SysTick->CTRL &= ~(1<<0); 
}

//*******************************************************************************
/*
��������:SysTick_Handler
��������:�жϺ���
������������
��������ֵ����
*/
void SysTick_Handler (void)
{
	if (systick_time > 0)
	{
			systick_time--;
	}	
}


