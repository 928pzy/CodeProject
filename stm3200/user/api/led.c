#include "stm32f10x.h"
#include "led.h"



//**********************************************************************************
/*
函数名称:Delay
函数功能:延时函数
函数参数：无
函数返回值：无
*/
static void Delay( time)
{
	while (time--);
}	

//*************************************************************************************

/*
函数名称:LED1_Config
函数功能:初始化
函数参数：无
函数返回值：无
*/

//PB5灯亮是需要的模式
void LED1_Config(void)
{
	//打开B端口时钟第三位置1
	RCC->APB2ENR |= (1<<3);
	//初始化PB5位为通用推挽输出
	//先清零20~23位，在赋值
	GPIOB->CRL &= ~(0xf<<4*5);
	GPIOB->CRL |= (0X3<<20);
}

//************************************************************************************
//PE5灯亮时需要的模式
void LED2_Config(void)
{
	//打开E端口时钟第三位置1，其他不变
	RCC->APB2ENR |= (1<<6);
	//初始化PB5位为通用推挽输出
	//先清零20~23位，在赋值
	GPIOE->CRL &= ~(0xf<<4*5);
	GPIOE->CRL |= (0X3<<20);
}

//PA5灯亮时需要的模式
void LED3_Config(void)
{
	//打开A端口时钟第三位置1，其他不变
	RCC->APB2ENR |= (1<<2);
	//初始化PB5位为通用推挽输出
	//先清零20~23位，在赋值
	GPIOA->CRL &= ~(0xf<<4*5);
	GPIOA->CRL |= (0X3<<20);
}

//PA6灯亮时需用的模式
void LED4_Config(void)
{
	//打开B端口时钟第三位置1，其他不变
	RCC->APB2ENR |= (1<<2);
	//初始化PB5位为通用推挽输出
	//先清零20~23位，在赋值
	GPIOA->CRL &= ~(0xf<<4*6);
	GPIOA->CRL |= (0X3<<24);
}

//***************************************************************************************
/*
函数名称:Led_cnf
函数功能:初始化
函数参数：无
函数返回值：无
*/
void Led_cnf (void)
{
	RCC->APB2ENR |= (1<<3);
	GPIOB->CRL &= ~(0xf<<4*5);
	GPIOB->CRL |= (0X3<<20);
	
	RCC->APB2ENR |= (1<<6);
	GPIOE->CRL &= ~(0xf<<4*5);
	GPIOE->CRL |= (0X3<<20);
	
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &= ~(0xf<<4*5);
	GPIOA->CRL |= (0X3<<20);
	
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &= ~(0xf<<4*6);
	GPIOA->CRL |= (0X3<<24);
	LED1(0);LED2(0);LED3(0);LED4(0);
}

//*************************************************************************************************
/*
函数名称:LED_ss（采用三目运算符实现流水灯）
函数功能:流水灯
函数参数：无
函数返回值：无
*/
void LED_ss (void)
{
	while (1)
	{
		LED1(1);LED2(0);LED3(0);LED4(0);
		Delay(1000000);
		LED1(0);LED2(1);LED3(0);LED4(0);
		Delay(1000000);
		LED1(0);LED2(0);LED3(1);LED4(0);
		Delay(1000000);
		LED1(0);LED2(0);LED3(0);LED4(1);
		Delay(1000000);
	}
}

//**************************************************************************************
/*
函数名称:LED_hx（采用三目运算符实现呼吸灯）
函数功能:呼吸灯
函数参数：无
函数返回值：无
*/
void LED_hx (void)
{
	u16 i = 0;
	while (1)
	{
		for (i = 0;i < 1500;i++)
		{
			LED1(1);LED2(1);LED3(1);LED4(1);
			Delay(i);
			LED1(0);LED2(0);LED3(0);LED4(0);
			Delay(1500-i);
		}
		for (i = 0;i < 1500;i++)
		{
			LED1(0);LED2(0);LED3(0);LED4(0);
			Delay(i);
			LED1(1);LED2(1);LED3(1);LED4(1);
			Delay(1500-i);
		}
	}
}
