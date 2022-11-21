#include "includes.h"
#include "stdio.h"


/*************************************************************************************
函数名称:TIM3_Config
函数功能:（TIM3-CH2通道）的初始化
函数参数：无
函数返回值：无
***************************************************************************************/
void TIM3_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
		//打开时钟 GPIOA 、IIM3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//初始化GPIO口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//初始化NVIC通道
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//初始化TIM3时钟
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  //打开通道2
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //设置2通道上的比较2寄存器捕获上升沿
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  //IC1映射到TI2通道（目的：只使用比较1寄存器）
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //无需把捕获到的频率分频
  TIM_ICInitStructure.TIM_ICFilter = 0x0;   //不滤波
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
函数名称: TIM3_IRQHandler  
函数功能: 中断服务函数
函数参数：无
函数返回值：无
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
			printf("输入PWM波的频率为：%.2fKHz\r\n",(float)(Frequency/1000));
			printf("输入PWM波的占空比为%f\r\n",DutyCycle*100);
		  printf("tim3_num = %d\r\n",tim3_num);
}


























