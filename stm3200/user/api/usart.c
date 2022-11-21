/*

1、需打开串口时钟、利用函数使能串口、检测标志位及对标志位清“0”。

*/
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "beep.h"
#include "string.h"

static u8 i = 0;
char usart1_ch[3]={'\0'};
char usart1_flag=0;

//*********************************************************************************
/*
函数名称:USART1_Config
函数功能:USART1的初始化
函数参数：无
函数返回值：无
*/

void USART1_Config (void)
{
	   //定义结构体
	USART_InitTypeDef USART_InitStruct;
	  //初始化GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	
	
			//打开时钟 GPIOA  USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	                          
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //初始化GPIO口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//初始化结构体
	USART_InitStruct.USART_BaudRate = 115200;//波特率选择
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//不知道
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //模式的选择
	USART_InitStruct.USART_Parity = USART_Parity_No;                             //奇偶校验的选择      
	USART_InitStruct.USART_StopBits = USART_StopBits_1;                          //停止位数的选择
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     //传输数据位数的选择
	
	//对串口1初始化
	USART_Init(USART1,&USART_InitStruct);
	
	USART_Cmd(USART1,ENABLE);//使能 USART1
}

//*********************************************************************************************

/*
函数名称:AFIO_CYS
函数功能:重映射到GPIOB,重新初始化及使能串口
函数参数：无
函数返回值：无
*/

void AFIO_CYS (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
		//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_USART1,ENABLE);
	
	 //更改指定管脚映射  TX  PB6   RX   PB7
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	//配置GPIO端口6  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//配置GPIO端口7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//配置USART
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	//初始化USART1
	USART_Init(USART1,&USART_InitStruct);
	
	//使能串口
	USART_Cmd(USART1,ENABLE);
}

//********************************************************************************

/*
函数名称:USART1_Zhong
函数功能：串口中断
函数参数：无
函数返回值：无
*/
void USART1_Zhong (void)
{
	   //定义结构体
	USART_InitTypeDef USART_InitStruct;
	  //初始化GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//打开GPIO、串口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	
	//使能串口中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//初始化GPIO口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //初始化GPIO口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//初始化串口
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength =USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
	
	
	
	//初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
}

//**********************************************************************************

//编写 fputc 函数
int fputc(int c,FILE * fp)
{
	//判断标志是否为空
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE )== RESET);
	//发送数据
	USART_SendData(USART1,c);
	
	//判断标志是否为空
//	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE )== RESET);
	
	return c;
}

//***************************************************************************************
/*
函数名称:USART_Echo
函数功能:回显
函数参数：无
函数返回值：无
*/

void USART_Echo (void)
{
	u8 ch = 0;//保存接收到的数据
	
	//等待接受到的数据
	while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	
	//把接收到的数据保存
	ch = USART_ReceiveData(USART1);
	//把接受到的数据发送回去
	USART_SendData(USART1,ch);
	//等待发送完成
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

//***********************************************************************************

/*
函数名称:USART_js
函数功能:通过USART1接受数据
函数参数：无
函数返回值：无
*/
void USART_js (void)
{
	u8 i = 0xff;
	
	if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
	 i = USART_ReceiveData(USART1);
		printf("%c",i);
		if (i == '1')
		{
			BUZZER(1);
		}
		else 
		{
			BUZZER(0);
		}
	}
}

//********************************************************************************

/*
函数名称:USART_Fun2
函数功能:上位机发送数据，下位机接受数据并解析，操作相关外部设备
函数参数：无
函数返回值：无
*/
void USART_Fun2 (void)
{
	u8 i = 0xff;
	while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	
	i = USART_ReceiveData(USART1);
	
	if (i > '1')
	{
		LED1(1);
	}
	else 
	{
		LED1(0);
	}
}

//*******************************************************************************
/*
函数名称:USART_ZFC
函数功能:通过发送字符串控制灯亮
函数参数：无
函数返回值：无
*/
char str[3];
void USART_ZFC (void)
{
		static	u32 i = 0;
	
		while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		
		str[i++] = USART_ReceiveData(USART1);
		
		if (i == 2)
		{
				i = 0;
		}
		
		if (strcmp((const char *)str,"12") == 0)
		{
				LED1(1);
		}
		else
		{
				LED1(0);
		}
}

//**********************************************************************************
/*
函数名称:AFIO_HX
函数功能:重映射到GPIOB,实现回显功能
函数参数：无
函数返回值：无
*/
void AFIO_HX (void)
{
		u8 i = 0xff;
		//接收
		while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		i = USART_ReceiveData(USART1);

		//发送
		USART_SendData(USART1,i);
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

//*****************************************************************************************
/*
函数名称:AFIO_ZFSZ
函数功能:字符串控制灯
函数参数：无
函数返回值：无
*/
void AFIO_ZFSZ (void)
{
	char str[10];
	static u32 i = 0;
		while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		str[i++] = USART_ReceiveData(USART1);
		if (i == 3)
		{
				i = 0;
		}
	if (strcmp ((const char *)str,"123") == 0)
	{
			BUZZER(1);
	}
	if (strcmp ((const char *)str,"456") == 0)
	{
			BUZZER(0);
	}
}

//*******************************************************************************

/*
函数名称:USART_SC
函数功能:利用上层协议实现，控制四个灯一个蜂鸣器
函数参数：无
函数返回值：无
*/

void USART_SC (void)
{
	char str[10];
	u32 i = 0;
	for (i = 0;i < 2;i++)
	{
	 while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		str[i] = USART_ReceiveData(USART1);
		printf("%c",str[i]);
	}
	
	if (strcmp((const char *)str,"11") == 0)
	{
			LED1(1);
	}
	else if (strcmp ((const char *)str,"10") == 0)
	{ 
			LED1(0);
	}
	else if (strcmp ((const char *)str,"21") == 0)
	{
			LED2(1);
	}
	else if (strcmp ((const char *)str,"20") == 0)
	{
			LED2(0);
	}
	else if (strcmp ((const char *)str,"31") == 0)
	{
			LED3(1);
	}
	else if (strcmp ((const char *)str,"30") == 0)
	{
			LED3(0);
	}
	else if (strcmp ((const char *)str,"41") == 0)
	{
			LED4(1);
	}
	else if (strcmp ((const char *)str,"40") == 0)
	{
			LED4(0);
	}
	else if (strcmp ((const char *)str,"51") == 0)
	{
			BUZZER(1);
	}
	else if (strcmp ((const char *)str,"50") == 0)
	{
			BUZZER(0);
	}
}

//****************************************************************************
/*
函数名称:USART1_IRQHandler
函数功能：串口中断函数
函数参数：无
函数返回值：无
*/
/*void USART1_IRQHandler(void)
{
		u8 i = 0xff;
	if (USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
		i = USART_ReceiveData(USART1);
	}
		if (i == '1')
			{
							BUZZER(1);
			}
			else
			{
						BUZZER(0);
			}
}*/
	
//************************************************************************************
/*
函数名称:USART1_IRQHandler
函数功能：串口中断函数
函数参数：无
函数返回值：无
*/

void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除标志位
		
			usart1_ch[i++]= USART_ReceiveData(USART1);//接受数据
		 
			if(i == 2)
			{
				usart1_flag = 1;
				i = 0;
			}
			USART1_TEXT();
}

//*********************************************************************************
/*
函数名称:USART1_TEXT
函数功能：串口中断函数
函数参数：无
函数返回值：无
*/
void USART1_TEXT (void)
{
		if (usart1_flag == 1)
		{
				if ((strcmp((const char *)usart1_ch,"12") == 0)&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8)==RESET)
				{
						BUZZER(1);
						usart1_flag = 0;
				}
				else if ((strcmp((const char *)usart1_ch,"13") == 0)&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8)==SET)
				{
						BUZZER(0);
						usart1_flag = 0;
				}
		}
}



