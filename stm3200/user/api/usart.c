/*

1����򿪴���ʱ�ӡ����ú���ʹ�ܴ��ڡ�����־λ���Ա�־λ�塰0����

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
��������:USART1_Config
��������:USART1�ĳ�ʼ��
������������
��������ֵ����
*/

void USART1_Config (void)
{
	   //����ṹ��
	USART_InitTypeDef USART_InitStruct;
	  //��ʼ��GPIO��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	
			//��ʱ�� GPIOA  USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	                          
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//��ʼ���ṹ��
	USART_InitStruct.USART_BaudRate = 115200;//������ѡ��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//��֪��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //ģʽ��ѡ��
	USART_InitStruct.USART_Parity = USART_Parity_No;                             //��żУ���ѡ��      
	USART_InitStruct.USART_StopBits = USART_StopBits_1;                          //ֹͣλ����ѡ��
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     //��������λ����ѡ��
	
	//�Դ���1��ʼ��
	USART_Init(USART1,&USART_InitStruct);
	
	USART_Cmd(USART1,ENABLE);//ʹ�� USART1
}

//*********************************************************************************************

/*
��������:AFIO_CYS
��������:��ӳ�䵽GPIOB,���³�ʼ����ʹ�ܴ���
������������
��������ֵ����
*/

void AFIO_CYS (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
		//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_USART1,ENABLE);
	
	 //����ָ���ܽ�ӳ��  TX  PB6   RX   PB7
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	//����GPIO�˿�6  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//����GPIO�˿�7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//����USART
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	//��ʼ��USART1
	USART_Init(USART1,&USART_InitStruct);
	
	//ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
}

//********************************************************************************

/*
��������:USART1_Zhong
�������ܣ������ж�
������������
��������ֵ����
*/
void USART1_Zhong (void)
{
	   //����ṹ��
	USART_InitTypeDef USART_InitStruct;
	  //��ʼ��GPIO��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//��GPIO�����ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	
	//ʹ�ܴ����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//��ʼ������
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength =USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
	
	
	
	//��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
}

//**********************************************************************************

//��д fputc ����
int fputc(int c,FILE * fp)
{
	//�жϱ�־�Ƿ�Ϊ��
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE )== RESET);
	//��������
	USART_SendData(USART1,c);
	
	//�жϱ�־�Ƿ�Ϊ��
//	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE )== RESET);
	
	return c;
}

//***************************************************************************************
/*
��������:USART_Echo
��������:����
������������
��������ֵ����
*/

void USART_Echo (void)
{
	u8 ch = 0;//������յ�������
	
	//�ȴ����ܵ�������
	while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	
	//�ѽ��յ������ݱ���
	ch = USART_ReceiveData(USART1);
	//�ѽ��ܵ������ݷ��ͻ�ȥ
	USART_SendData(USART1,ch);
	//�ȴ��������
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

//***********************************************************************************

/*
��������:USART_js
��������:ͨ��USART1��������
������������
��������ֵ����
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
��������:USART_Fun2
��������:��λ���������ݣ���λ���������ݲ���������������ⲿ�豸
������������
��������ֵ����
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
��������:USART_ZFC
��������:ͨ�������ַ������Ƶ���
������������
��������ֵ����
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
��������:AFIO_HX
��������:��ӳ�䵽GPIOB,ʵ�ֻ��Թ���
������������
��������ֵ����
*/
void AFIO_HX (void)
{
		u8 i = 0xff;
		//����
		while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		i = USART_ReceiveData(USART1);

		//����
		USART_SendData(USART1,i);
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

//*****************************************************************************************
/*
��������:AFIO_ZFSZ
��������:�ַ������Ƶ�
������������
��������ֵ����
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
��������:USART_SC
��������:�����ϲ�Э��ʵ�֣������ĸ���һ��������
������������
��������ֵ����
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
��������:USART1_IRQHandler
�������ܣ������жϺ���
������������
��������ֵ����
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
��������:USART1_IRQHandler
�������ܣ������жϺ���
������������
��������ֵ����
*/

void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);//�����־λ
		
			usart1_ch[i++]= USART_ReceiveData(USART1);//��������
		 
			if(i == 2)
			{
				usart1_flag = 1;
				i = 0;
			}
			USART1_TEXT();
}

//*********************************************************************************
/*
��������:USART1_TEXT
�������ܣ������жϺ���
������������
��������ֵ����
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



