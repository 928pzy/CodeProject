#include "includes.h"

/*************************************************************************************
函数名称:USART_485
函数功能:通过485实现串口通信
函数参数：无
函数返回值：无
***************************************************************************************/
void USART_485 (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
		//打开GPIO口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	  //初始化GPIO口为输出模----PG9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}

/*************************************************************************************
函数名称:USART2_Config
函数功能:初始化USART2
函数参数：无
函数返回值：无
***************************************************************************************/
void USART2_Config (void)
{
	   //定义结构体
	USART_InitTypeDef USART_InitStruct;
	  //初始化GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	
			//打开时钟 GPIOA  USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;     //复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	                          
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; //初始化GPIO口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//初始化结构体
	USART_InitStruct.USART_BaudRate = 115200;//波特率选择
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//不知道
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //模式的选择
	USART_InitStruct.USART_Parity = USART_Parity_No;                             //奇偶校验的选择      
	USART_InitStruct.USART_StopBits = USART_StopBits_1;                          //停止位数的选择
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     //传输数据位数的选择
	//对串口2初始化
	USART_Init(USART2,&USART_InitStruct);
}

/****************************************************
函数名：RS485_NVICConfig
形参：无
返回值：无
函数功能：RS485的中断初始化
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
	
	//使能串口2中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	
}

/****************************************************
函数名：RS485Config
形参：无
返回值：无
函数功能：RS485的初始化
****************************************************/
void RS485Config(void)
{
	USART_485();   //初始化PG9
	USART2_Config();  //初始化串口2
	RS485_NVICConfig();  //初始化中断通道
	USART_Cmd(USART2,ENABLE);  //使能串口2
	USA_485(0);  //接收使能
}

/*******************************************************************************************************
函数名称:USART_Get_485
函数功能:接收到的数据通过串口打印出来显
函数参数：无
函数返回值：无
*********************************************************************************************************/
void USART_Get_485 (void)
{
	u8 ch = 0;//保存接收到的数据
	
	//等待接受到的数据
	while (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == RESET);
	
	//清标志位
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	
	//把接收到的数据保存
	ch = USART_ReceiveData(USART2);
	
	printf("接受完成");
	printf("%d\r\n",ch);
//	//把接受到的数据发送回去
//	USART_SendData(USART1,ch);
//	//等待发送完成
//	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/*******************************************************************************************************
函数名称:USART_Send_485
函数功能:发送数据
函数参数：无
函数返回值：无
*********************************************************************************************************/
void USART_Send_485 (void)
{
	u8 ch = 1;
	//把接受到的数据发送回去
	USART_SendData(USART2,ch);
	//等待发送完成
	while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	printf("发送完成");
}

/*********************************************************************************************************
函数名：RS485QUEUE_Receive
形参：  int ch ：需要存入队列的数据
返回值： 无
函数功能： 队列的接收
*********************************************************************************************************/
QUEUElen RS485_QUEUElen;
void RS485QUEUE_Receive (int ch)
{
		if (RS485_QUEUElen.indata >= RS485_QUEUElen.end)     //判断读取指针有没有到达最后的地址
		{
				RS485_QUEUElen.indata = RS485_QUEUElen.front;    //如果已经到达最后的地址，就使开始地址赋给读取指针
		}
		if (RS485_QUEUElen.lenth < MAXSIZE)                  //判断存入数组的数据长度是否超过数组定义的大小
		{
				*RS485_QUEUElen.indata = ch;                     //没有超过，就接着存储
				RS485_QUEUElen.indata++;
				RS485_QUEUElen.lenth++;
		}
		else
		{
				printf("循环队列已满");
		}
}

/****************************************************
函数名：RS485QUEUE_Transmit
形参：无
返回值：返回从队列中读取到的值
函数功能：队列往外发送
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
			printf("队列以空");
	}
}

/***************************************************************************************************************
函数名：RS485QUEUE_init
形参：无
返回值：无
函数功能：队列的初始化
***************************************************************************************************************/
void RS485QUEUE_init (void)
{
		RS485_QUEUElen.front = RS485_QUEUElen.elem;     //指针指向数组的首地址
		RS485_QUEUElen.end = RS485_QUEUElen.front + MAXSIZE*4;   //指针类型大小为4，末地址为：首地址+数据长度*4
		RS485_QUEUElen.indata = RS485_QUEUElen.front;   //读取指针指向首地址
		RS485_QUEUElen.outdata = RS485_QUEUElen.front;
		RS485_QUEUElen.lenth = 0;    //数组中存储的字节长度为“0”
}

/******************************************************************************************************************
函数名：USART2_IRQHandler
形参：无
返回值：无
函数功能：中断服务函数
*******************************************************************************************************************/
void USART2_IRQHandler (void)
{
		u8 ch;
		if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
		{
				USA_485(0);
					//清标志位
				USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			
				ch = USART_ReceiveData(USART2);
					//把通过串口接受到的数据存到队列中
				RS485QUEUE_Receive(ch);
		}
}






