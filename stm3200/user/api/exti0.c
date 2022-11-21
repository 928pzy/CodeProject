/* 
1����ʱ�� (GPIO��AFIO)

2��ӳ��EXTI0��GPIO�� 
								��EXTI0û�ж�Ӧ��GPIO��-------������AFIO-CRx�Ĵ���ӳ�䵽GPIO���ϣ�

3����ʼ��GPIO�ڡ�EXTI0��NVIC���жϹ�������
									��EXTI0��NVIC��ʹ�ܶ��ڶ���Ľṹ���н��в�����

ע�⣺
			Ӧ���ж�ʱ�������������н������ȼ����顣��NVIC_PriorityGroupConfig��
			ֻ���豸����ʱ�ӣ��ж�û��ʱ�ӡ�
			�жϺ����������������á�
*/


#include "stm32f10x.h"
#include "exti0.h"
#include "stdio.h"
#include "usart.h"
#include "beep.h"
#include "led.h"
#include "string.h"

//*************************************************************************
/*
��������:Exti0_Config
��������:��ʼ���ж�EXTI0
������������
��������ֵ����
*/
void Exti0_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;//NVIC
	
	//��ʱ��GPIO��AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
		//ӳ��EXTI0��GPIOA
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	//��ʼ��GPIO�˿�
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//��ʼ��EXTI
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//ѡ���ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//ѡ�񴥷��ж�ģʽ
	EXTI_Init(&EXTI_InitStruct);
	
	//��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//�ж�EXTI0ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//ѡ��ռ�����ȼ��ĵȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//ѡ��μ����ȼ��ĵȼ�

	NVIC_Init(&NVIC_InitStruct);	
}

//*********************************************************************************
/*
��������:EXTI0_IRQHandler
��������:�ⲿ�жϺ���
������������
��������ֵ����
*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
		EXTI_ClearITPendingBit(EXTI_Line0);
	
	printf("�����ж�\r\n");
}
	





	




