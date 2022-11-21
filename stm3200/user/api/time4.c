#include "includes.h"


/*********************************************************************************
��������:TIM4_Config
��������:TIM4�ĳ�ʼ��
����������prescaler -- ��Ƶֵ
					 period -- ��װ��ֵ
��������ֵ����
**********************************************************************************/
  void TIM4_Config (u16 prescaler,u16 period)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_OCInitTypeDef	TIM_OCInitStructure;
	
		//��ʱ��----TIM4 �� GPIO
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		//��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	 //TIM4ʱ�ӳ�ʼ����Ƶ 
	TIM_TimeBaseStructure.TIM_Period = period;   //��ֵװ��
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;  //��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //ѡ���������ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   //��ʼ��ʱ�������λ
	 
	 //��ʼ��ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ��
  TIM_OCInitStructure.TIM_Pulse = period/2;    //�ȽϼĴ�����װ��ֵ
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //��Ч��ƽΪ�ߵ�ƽ
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);   //ʹ��Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
//	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);  //ʹ�ܱȽ�1���ж�Դ
}

/*********************************************************************************
��������:NVIC_Config
��������:��ʼ���ж�ͨ��
������������
��������ֵ����
**********************************************************************************/
void NVIC4_Config (void)
{
		NVIC_InitTypeDef NVIC_InitStruct;
		//��ʼ��NVIC
		NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//���ж�ͨ��
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//ѡ��ռ�����ȼ��ĵȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//ѡ��μ����ȼ��ĵȼ�
		NVIC_Init(&NVIC_InitStruct);	
}

/*********************************************************************************
��������:TIM4_IRQHandler
��������:TIM4���жϺ���
������������
��������ֵ����
**********************************************************************************/
//void TIM4_IRQHandler (void)
//{
//	static u32 flag = 100;
//	if (TIM_GetITStatus(TIM4,TIM_IT_CC1) == SET)
//	{
//		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
//		TIM_SetCompare1(TIM4,flag--);
//		while (flag == 0)
//			flag = 100;
//	}
//}
