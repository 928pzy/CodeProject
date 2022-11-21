#include "includes.h"


extern int A,B,C,D,E,F,K;
/*
IIC初始化：
1、根据数据手册找到GPIO口
					把GPIO口配置为开漏输出型
2、打开IIC、GPIO口的时钟
3、初始化IIC、GPIO口
*/

/**********************************************************
函数名：IIC_Init
形参：无
返回值：无
函数功能：IIC初始化
**************************************************************************/
void IIC_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
		//打开IIC、GPIO口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
		//定义结构体，初始化GPIO、IIC
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;  //复用开漏输出（只输出低电平，因IIC有上拉电阻，平时都为高电平）
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;   //选择有应答信号
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //地址的位数
	I2C_InitStruct.I2C_ClockSpeed = 200000;  //时钟的速度
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_16_9; //配置高低电平的比例
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C; //选择IIC模式
	I2C_InitStruct.I2C_OwnAddress1 = 0x0A; //选择从机的地址
	I2C_Init(I2C2,&I2C_InitStruct);

	I2C_Cmd(I2C2,ENABLE); //使能IIC
}


/*
1、判断IIC、SDA线上是否忙碌
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
2、发送开始信号
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
3、等待开始信号完成
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
4、发送器件地址
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction);
5、判断地址是否发送完成
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
6、发送8位数据（代表字地址）
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
7、等待发送完成
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
8、发送最多8个字节的数据
9、产生停止信号
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
*/

/**************************************************************************************
函数名：IIC_WritePage
形参：Addr -- 从机地址
			Subaddr -- 从机字地址
			Lenth-- 需发送的字符的个数
返回值：无
函数功能：页写
***************************************************************************************/
void IIC_WritePage(u8 Addr,u8 Subaddr,u8 *Data,u16 Lenth)
{
	u16 i;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));  //判断总线标志位是否忙碌，忙碌为“1”，不忙为“0”
	
	I2C_GenerateSTART(I2C2,ENABLE);  //开始发送起始条件
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);  
	
	I2C_Send7bitAddress(I2C2,Addr,I2C_Direction_Transmitter);  //发送7位地址（设备地址）
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	I2C_SendData(I2C2,Subaddr); //发送字地址
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	
	for(i=0;i<Lenth;i++)   //发送数据
	{
		I2C_SendData(I2C2,Data[i]);
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	}
	
	I2C_GenerateSTOP(I2C2,ENABLE);  //产生传输停止的条件
	
}

/*
顺序读：
1、检测SDA是否忙碌
2、开始信号
3、检测开始信号发送完成
4、发送器件地址
5、检测器件地址发送完成
6、发送字地址
7、检测字地址发送完成
8、开始信号
9、检测开始信号发送完成
10、发送器件地址（改为读模式）//事件6的接收模式
11、检测器件发送地址完成
12、判断从机是否发送回来数据
13、读取数据
14、重复接受到（Lenth-1）次
15、最后一次数据接受不让STM32给予AT24C02发送应答信号
16、接受最后的数据
17、产生停止信号
18、打开应答使能
*/

/********************************************************************************************
函数名：IIC_Roundread
形参：Addr -- 从机地址
			Subaddr -- 字地址
			Lenth-- 读取的数据长度
返回值：无
函数功能：AT24C02的顺序读
**********************************************************************************************/
void IIC_Roundread(u8 Addr,u8 Subaddr,u8 *Data,u16 Lenth)
{
	u16 i;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));  //判断总线标志位是否忙碌
	
	I2C_GenerateSTART(I2C2,ENABLE); // 发送开始条件
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2,Addr,I2C_Direction_Transmitter); //发送7位地址（设备地址）---以及写指令
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	I2C_SendData(I2C2,Subaddr);  //发送字地址
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	
	I2C_GenerateSTART(I2C2,ENABLE);  //发送开始条件
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2,Addr,I2C_Direction_Receiver);  //发送7位地址（设备地址）--以及读命令
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
	
	for(i=0;i< Lenth-1;i++)
	{
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
		Data[i] = I2C_ReceiveData(I2C2);
	}
	
	I2C_AcknowledgeConfig(I2C2,DISABLE);  //关闭自动应答使能
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
	Data[i] = I2C_ReceiveData(I2C2);      //接受最后一个数据不许给予从机回应
	
	I2C_GenerateSTOP(I2C2,ENABLE);   //发送停止位
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);  //打开应答功能
}

/*****************************************************************
1、通过对AT24C02页写，把256个字节的空间写满
2、从AT24C02中通过顺序读，把256个字节读出
*************************************************************************/
/*******************************************************************************
函数名：IIC_Test
形参：无
返回值：无
函数功能：测试IIC函数
********************************************************************************/
void IIC_Test (void)
{
	u16 i;
	u8 buffer[256];
	printf("IIC2测试开始\n\r");
	
	for (i = 0;i < 256;i++)
	{
			buffer[i] = 0;
	}
	IIC_Display(buffer);
	printf("开始写操作\n\r");
	printf("正在写操作.....\n\r");
	
	for(i=0;i<256;i++)
	{
		IIC_WritePage(0xA0,i,&buffer[i],1);
		printf("ok\t");
		Delay_ms(50);
	}
	
	printf("写操作完成\n\r");
	Clear_buffer(buffer,256);
	printf("读操作开始\n\r");
	IIC_Roundread(0xA0,0,buffer,256);
	printf("读操作完成\n\r");
	
	IIC_Display(buffer); //打印读出来的数据
	printf("IIC操作完成\n\r");
}

/*****************************************************************************************
函数名：Clear_buffer
形参：
返回值：无
函数功能：给一个数组全部赋值为“0”
*****************************************************************************************/
void Clear_buffer (u8 *Buffer,u16 Lenth)
{
	u16 i;
	for(i=0;i<Lenth;i++)
	{
		Buffer[i] = 0;
	}
}

/****************************************************************************************
函数名:Display_buffer
形参：buffer要显示的数组的地址
返回值：无
函数功能：串口打印一个数组
****************************************************************************************/
void IIC_Display(u8 *buffer)
{
	int iic_x,iic_y;
	for(iic_y = 0;iic_y<16;iic_y++)
	{
		for(iic_x=0;iic_x<16;iic_x++)
		{
			printf("%x\t",*buffer++);
		}
		printf("\n\r");
	}
}

/****************************************************************************************
函数名:IIC_Sive
形参：无
返回值：无
函数功能：储存A、B、C....的值到W24C06
****************************************************************************************/
void IIC_Sive (void)
{
	union INTTOCHAR
	{
		int a;
		u8 str[4];
	};
	union INTTOCHAR temp1;
	u16 i,k = 0,j = 0;
	int num[8];
	u8 str2[255];
	num[0] = 1;
	num[1] = A;
	num[2] = B;
	num[3] = C;
	num[4] = D;
	num[5] = E;
	num[6] = F;
	num[7] = K;
	printf("IIC2测试开始\n\r");
	printf("开始写操作\n\r");
	printf("正在写操作.....\n\r");
	for (j = 0;j < 8;j++)
	{
		temp1.a = num[j];
		for(i = 0;i<4;i++)
		{
			IIC_WritePage(0xA0,k,&temp1.str[i],1);
			printf("ok\t");
			Delay_ms(50);
			k++;
		}
	}
	printf("写操作完成\n\r");
	
	
	printf("读操作开始\n\r");
	IIC_Roundread(0xA0,0,str2,1);
	printf("读操作完成\n\r");
	
	IIC_Display(str2); //打印读出来的数据
	printf("IIC操作完成\n\r");
	
}





































