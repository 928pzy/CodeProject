#include "includes.h"


/*********************************************************************************
函数名称:TIM4_Config
函数功能:TIM4的初始化
函数参数：prescaler -- 分频值
					 period -- 重装载值
函数返回值：无
**********************************************************************************/
  void TIM4_Config (u16 prescaler,u16 period)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_OCInitTypeDef	TIM_OCInitStructure;
	
		//打开时钟----TIM4 、 GPIO
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		//初始化GPIO口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	 //TIM4时钟初始化分频 
	TIM_TimeBaseStructure.TIM_Period = period;   //阈值装载
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;  //分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //选择计数器的模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   //初始化时间基数单位
	 
	 //初始化通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能
  TIM_OCInitStructure.TIM_Pulse = period/2;    //比较寄存器的装载值
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平为高电平
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);   //使能预装载寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
//	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);  //使能比较1的中断源
}

/*********************************************************************************
函数名称:NVIC_Config
函数功能:初始化中断通道
函数参数：无
函数返回值：无
**********************************************************************************/
void NVIC4_Config (void)
{
		NVIC_InitTypeDef NVIC_InitStruct;
		//初始化NVIC
		NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//打开中断通道
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//选择占先优先级的等级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//选择次级优先级的等级
		NVIC_Init(&NVIC_InitStruct);	
}

/*********************************************************************************
函数名称:TIM4_IRQHandler
函数功能:TIM4的中断函数
函数参数：无
函数返回值：无
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
