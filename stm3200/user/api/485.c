#include "includes.h"

/*************************************************************************************
��������:USART_485
��������:ͨ��485ʵ�ִ���ͨ��
������������
��������ֵ����
***************************************************************************************/
void USART_485 (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
		//��GPIO�ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	  //��ʼ��GPIO��Ϊ���ģ----PG9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}

/*************************************************************************************
��������:USART2_Config
��������:��ʼ��USART2
������������
��������ֵ����
***************************************************************************************/
void USART2_Config (void)
{
	   //����ṹ��
	USART_InitTypeDef USART_InitStruct;
	  //��ʼ��GPIO��
	GPIO_InitTypeDef GPIO_InitStruct;
	
			//��ʱ�� GPIOA  USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	                          
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; //��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//��ʼ���ṹ��
	USART_InitStruct.USART_BaudRate = 115200;//������ѡ��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//��֪��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //ģʽ��ѡ��
	USART_InitStruct.USART_Parity = USART_Parity_No;                             //��żУ���ѡ��      
	USART_InitStruct.USART_StopBits = USART_StopBits_1;                          //ֹͣλ����ѡ��
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     //��������λ����ѡ��
	//�Դ���2��ʼ��
	USART_Init(USART2,&USART_InitStruct);
}

/****************************************************
��������RS485_NVICConfig
�βΣ���
����ֵ����
�������ܣ�RS485���жϳ�ʼ��
****************************************************/
void RS485_NVICConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//ʹ�ܴ���2�ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	
}

/****************************************************
��������RS485Config
�βΣ���
����ֵ����
�������ܣ�RS485�ĳ�ʼ��
****************************************************/
void RS485Config(void)
{
	USART_485();   //��ʼ��PG9
	USART2_Config();  //��ʼ������2
	RS485_NVICConfig();  //��ʼ���ж�ͨ��
	USART_Cmd(USART2,ENABLE);  //ʹ�ܴ���2
	USA_485(0);  //����ʹ��
}

/*******************************************************************************************************
��������:USART_Get_485
��������:���յ�������ͨ�����ڴ�ӡ������
������������
��������ֵ����
*********************************************************************************************************/
void USART_Get_485 (void)
{
	u8 ch = 0;//������յ�������
	
	//�ȴ����ܵ�������
	while (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == RESET);
	
	//���־λ
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	
	//�ѽ��յ������ݱ���
	ch = USART_ReceiveData(USART2);
	
	printf("�������");
	printf("%d\r\n",ch);
//	//�ѽ��ܵ������ݷ��ͻ�ȥ
//	USART_SendData(USART1,ch);
//	//�ȴ��������
//	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/*******************************************************************************************************
��������:USART_Send_485
��������:��������
������������
��������ֵ����
*********************************************************************************************************/
void USART_Send_485 (void)
{
	u8 ch = 1;
	//�ѽ��ܵ������ݷ��ͻ�ȥ
	USART_SendData(USART2,ch);
	//�ȴ��������
	while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	printf("�������");
}

/*********************************************************************************************************
��������RS485QUEUE_Receive
�βΣ�  int ch ����Ҫ������е�����
����ֵ�� ��
�������ܣ� ���еĽ���
*********************************************************************************************************/
QUEUElen RS485_QUEUElen;
void RS485QUEUE_Receive (int ch)
{
		if (RS485_QUEUElen.indata >= RS485_QUEUElen.end)     //�ж϶�ȡָ����û�е������ĵ�ַ
		{
				RS485_QUEUElen.indata = RS485_QUEUElen.front;    //����Ѿ��������ĵ�ַ����ʹ��ʼ��ַ������ȡָ��
		}
		if (RS485_QUEUElen.lenth < MAXSIZE)                  //�жϴ�����������ݳ����Ƿ񳬹����鶨��Ĵ�С
		{
				*RS485_QUEUElen.indata = ch;                     //û�г������ͽ��Ŵ洢
				RS485_QUEUElen.indata++;
				RS485_QUEUElen.lenth++;
		}
		else
		{
				printf("ѭ����������");
		}
}

/****************************************************
��������RS485QUEUE_Transmit
�βΣ���
����ֵ�����شӶ����ж�ȡ����ֵ
�������ܣ��������ⷢ��
****************************************************/
u8 RS485QUEUE_Transmit (void)
{
	u8 ch;
	if (RS485_QUEUElen.outdata >= RS485_QUEUElen.end)          
	{
			RS485_QUEUElen.outdata = RS485_QUEUElen.front;
	}
	if (RS485_QUEUElen.outdata != RS485_QUEUElen.indata)
	{
			ch = *RS485_QUEUElen.outdata;
			RS485_QUEUElen.outdata++;
			RS485_QUEUElen.lenth--;
			return ch;
	}
	else
	{
			printf("�����Կ�");
	}
}

/***************************************************************************************************************
��������RS485QUEUE_init
�βΣ���
����ֵ����
�������ܣ����еĳ�ʼ��
***************************************************************************************************************/
void RS485QUEUE_init (void)
{
		RS485_QUEUElen.front = RS485_QUEUElen.elem;     //ָ��ָ��������׵�ַ
		RS485_QUEUElen.end = RS485_QUEUElen.front + MAXSIZE*4;   //ָ�����ʹ�СΪ4��ĩ��ַΪ���׵�ַ+���ݳ���*4
		RS485_QUEUElen.indata = RS485_QUEUElen.front;   //��ȡָ��ָ���׵�ַ
		RS485_QUEUElen.outdata = RS485_QUEUElen.front;
		RS485_QUEUElen.lenth = 0;    //�����д洢���ֽڳ���Ϊ��0��
}

/******************************************************************************************************************
��������USART2_IRQHandler
�βΣ���
����ֵ����
�������ܣ��жϷ�����
*******************************************************************************************************************/
void USART2_IRQHandler (void)
{
		u8 ch;
		if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
		{
				USA_485(0);
					//���־λ
				USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			
				ch = USART_ReceiveData(USART2);
					//��ͨ�����ڽ��ܵ������ݴ浽������
				RS485QUEUE_Receive(ch);
		}
}






