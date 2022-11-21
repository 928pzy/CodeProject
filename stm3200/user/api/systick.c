/*
1、设置时钟源即为初始化系统时钟。设置完之后，需关闭时钟，防止时钟设置完之后开始计时。
*/


#include "systick.h"
#include "stm32f10x.h"

u32 systick_time = 0; //全局变量

#define  LOAD_1US      72
#define  LOAD_10US     720
#define  LOAD_100US    7200
#define  LOAD_1000US   72000

//**********************************************************************
/*
函数名称:Systick_Config
函数功能:初始化
函数参数：无
函数返回值：无
*/
void Systick_Config(void)
{
	//设置时钟源
	while (SysTick_Config(LOAD_1US) == 1);  //（LOAD_1US）为阈值，这条语句是判断阈值是否大于2^24
	//关闭时钟
	SysTick->CTRL &= ~(1<<0);				
}

//*******************************************************************************
/*
函数名称:Delay_us
函数功能:延时函数
函数参数：无
函数返回值：无
*/
void Delay_us (u32 time)
{
	systick_time = time;
	//打开时钟
	SysTick->CTRL |= (1<<0);
	
	while (systick_time);
	
	//关闭时钟
	SysTick->CTRL &= ~(1<<0); 
}

//*******************************************************************************
/*
函数名称:SysTick_Handler
函数功能:中断函数
函数参数：无
函数返回值：无
*/
void SysTick_Handler (void)
{
	if (systick_time > 0)
	{
			systick_time--;
	}	
}


