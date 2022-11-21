#include "stm32f10x.h"
#include "beep.h"
#include "led.h"

//****************************************************************************************
/*
函数名称:BEEP_config
函数功能:蜂鸣器的初始化
函数参数：无
函数返回值：无
*/
void BEEP_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//定义一个结构体变量，初始化
	//打开B端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//把PB8初始化为通用推挽模式50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//管脚初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//选择管教模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//管脚速度
	//对端口初始化
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}








