#include "includes.h"
#include "stdio.h"


/*************************************************************************************
��������:TIM3_Config
��������:��TIM3-CH2ͨ�����ĳ�ʼ��
������������
��������ֵ����
***************************************************************************************/
void TIM3_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
		//��ʱ�� GPIOA ��IIM3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//��ʼ��NVICͨ��
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//��ʼ��TIM3ʱ��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  //��ͨ��2
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //����2ͨ���ϵıȽ�2�Ĵ�������������
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  //IC1ӳ�䵽TI2ͨ����Ŀ�ģ�ֻʹ�ñȽ�1�Ĵ�����
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //����Ѳ��񵽵�Ƶ�ʷ�Ƶ
  TIM_ICInitStructure.TIM_ICFilter = 0x0;   //���˲�
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	
	/* Select the TIM3 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);
	
	/* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	 /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
	
		/* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	
	 /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

/*************************************************************************************
��������: TIM3_IRQHandler  
��������: �жϷ�����
������������
��������ֵ����
***************************************************************************************/
float IC2Value = 0;
float DutyCycle = 0;
float Frequency = 0;
float IC1Value = 0;
u32 tim3_num = 0;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_CC2) == SET)
	{
		/* Clear TIM3 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		/* Get the Input Capture value */
    IC2Value = TIM_GetCapture2(TIM3);
		if(IC2Value != 0)
		{
			IC1Value = TIM_GetCapture1(TIM3);
			/* Duty cycle computation */
			DutyCycle = (IC1Value)/IC2Value;

			/* Frequency computation */
			Frequency = 72000000 / IC2Value;
//			tim3_num = 0;
		}
		else
		{
			DutyCycle = 0;
			Frequency = 0;
		}		
	}
	printf("AAAAAAA");
	printf("IC1Value = %f\r\n",IC1Value);
			printf("IC2Value = %f\r\n",IC2Value);
			printf("����PWM����Ƶ��Ϊ��%.2fKHz\r\n",(float)(Frequency/1000));
			printf("����PWM����ռ�ձ�Ϊ%f\r\n",DutyCycle*100);
		  printf("tim3_num = %d\r\n",tim3_num);
}


























