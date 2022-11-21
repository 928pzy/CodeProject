/* 
1、打开时钟 (GPIO、AFIO)

2、映射EXTI0到GPIO口 
								（EXTI0没有对应的GPIO口-------需利用AFIO-CRx寄存器映射到GPIO口上）

3、初始化GPIO口、EXTI0、NVIC（中断管理器）
									（EXTI0、NVIC的使能都在定义的结构体中进行操作）

注意：
			应用中断时，需在主函数中进行优先级分组。（NVIC_PriorityGroupConfig）
			只有设备才有时钟，中断没有时钟。
			中断函数不声明，不调用。
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
函数名称:Exti0_Config
函数功能:初始化中断EXTI0
函数参数：无
函数返回值：无
*/
void Exti0_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;//NVIC
	
	//打开时钟GPIO、AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
		//映射EXTI0到GPIOA
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	//初始化GPIO端口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//初始化EXTI
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//选择中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//选择触发中断模式
	EXTI_Init(&EXTI_InitStruct);
	
	//初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//中断EXTI0通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//选择占先优先级的等级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//选择次级优先级的等级

	NVIC_Init(&NVIC_InitStruct);	
}

//*********************************************************************************
/*
函数名称:EXTI0_IRQHandler
函数功能:外部中断函数
函数参数：无
函数返回值：无
*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
		EXTI_ClearITPendingBit(EXTI_Line0);
	
	printf("触发中断\r\n");
}
	





	




