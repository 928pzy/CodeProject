#include "includes.h"


extern int A,B,C,D,E,F,K;
/*
IIC��ʼ����
1�����������ֲ��ҵ�GPIO��
					��GPIO������Ϊ��©�����
2����IIC��GPIO�ڵ�ʱ��
3����ʼ��IIC��GPIO��
*/

/**********************************************************
��������IIC_Init
�βΣ���
����ֵ����
�������ܣ�IIC��ʼ��
**************************************************************************/
void IIC_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
		//��IIC��GPIO�ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
		//����ṹ�壬��ʼ��GPIO��IIC
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;  //���ÿ�©�����ֻ����͵�ƽ����IIC���������裬ƽʱ��Ϊ�ߵ�ƽ��
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;   //ѡ����Ӧ���ź�
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //��ַ��λ��
	I2C_InitStruct.I2C_ClockSpeed = 200000;  //ʱ�ӵ��ٶ�
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_16_9; //���øߵ͵�ƽ�ı���
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C; //ѡ��IICģʽ
	I2C_InitStruct.I2C_OwnAddress1 = 0x0A; //ѡ��ӻ��ĵ�ַ
	I2C_Init(I2C2,&I2C_InitStruct);

	I2C_Cmd(I2C2,ENABLE); //ʹ��IIC
}


/*
1���ж�IIC��SDA�����Ƿ�æµ
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
2�����Ϳ�ʼ�ź�
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
3���ȴ���ʼ�ź����
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
4������������ַ
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction);
5���жϵ�ַ�Ƿ������
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
6������8λ���ݣ������ֵ�ַ��
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
7���ȴ��������
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
8���������8���ֽڵ�����
9������ֹͣ�ź�
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
*/

/**************************************************************************************
��������IIC_WritePage
�βΣ�Addr -- �ӻ���ַ
			Subaddr -- �ӻ��ֵ�ַ
			Lenth-- �跢�͵��ַ��ĸ���
����ֵ����
�������ܣ�ҳд
***************************************************************************************/
void IIC_WritePage(u8 Addr,u8 Subaddr,u8 *Data,u16 Lenth)
{
	u16 i;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));  //�ж����߱�־λ�Ƿ�æµ��æµΪ��1������æΪ��0��
	
	I2C_GenerateSTART(I2C2,ENABLE);  //��ʼ������ʼ����
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);  
	
	I2C_Send7bitAddress(I2C2,Addr,I2C_Direction_Transmitter);  //����7λ��ַ���豸��ַ��
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	I2C_SendData(I2C2,Subaddr); //�����ֵ�ַ
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	
	for(i=0;i<Lenth;i++)   //��������
	{
		I2C_SendData(I2C2,Data[i]);
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	}
	
	I2C_GenerateSTOP(I2C2,ENABLE);  //��������ֹͣ������
	
}

/*
˳�����
1�����SDA�Ƿ�æµ
2����ʼ�ź�
3����⿪ʼ�źŷ������
4������������ַ
5�����������ַ�������
6�������ֵ�ַ
7������ֵ�ַ�������
8����ʼ�ź�
9����⿪ʼ�źŷ������
10������������ַ����Ϊ��ģʽ��//�¼�6�Ľ���ģʽ
11������������͵�ַ���
12���жϴӻ��Ƿ��ͻ�������
13����ȡ����
14���ظ����ܵ���Lenth-1����
15�����һ�����ݽ��ܲ���STM32����AT24C02����Ӧ���ź�
16��������������
17������ֹͣ�ź�
18����Ӧ��ʹ��
*/

/********************************************************************************************
��������IIC_Roundread
�βΣ�Addr -- �ӻ���ַ
			Subaddr -- �ֵ�ַ
			Lenth-- ��ȡ�����ݳ���
����ֵ����
�������ܣ�AT24C02��˳���
**********************************************************************************************/
void IIC_Roundread(u8 Addr,u8 Subaddr,u8 *Data,u16 Lenth)
{
	u16 i;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));  //�ж����߱�־λ�Ƿ�æµ
	
	I2C_GenerateSTART(I2C2,ENABLE); // ���Ϳ�ʼ����
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2,Addr,I2C_Direction_Transmitter); //����7λ��ַ���豸��ַ��---�Լ�дָ��
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	I2C_SendData(I2C2,Subaddr);  //�����ֵ�ַ
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	
	I2C_GenerateSTART(I2C2,ENABLE);  //���Ϳ�ʼ����
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2,Addr,I2C_Direction_Receiver);  //����7λ��ַ���豸��ַ��--�Լ�������
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
	
	for(i=0;i< Lenth-1;i++)
	{
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
		Data[i] = I2C_ReceiveData(I2C2);
	}
	
	I2C_AcknowledgeConfig(I2C2,DISABLE);  //�ر��Զ�Ӧ��ʹ��
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
	Data[i] = I2C_ReceiveData(I2C2);      //�������һ�����ݲ������ӻ���Ӧ
	
	I2C_GenerateSTOP(I2C2,ENABLE);   //����ֹͣλ
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);  //��Ӧ����
}

/*****************************************************************
1��ͨ����AT24C02ҳд����256���ֽڵĿռ�д��
2����AT24C02��ͨ��˳�������256���ֽڶ���
*************************************************************************/
/*******************************************************************************
��������IIC_Test
�βΣ���
����ֵ����
�������ܣ�����IIC����
********************************************************************************/
void IIC_Test (void)
{
	u16 i;
	u8 buffer[256];
	printf("IIC2���Կ�ʼ\n\r");
	
	for (i = 0;i < 256;i++)
	{
			buffer[i] = 0;
	}
	IIC_Display(buffer);
	printf("��ʼд����\n\r");
	printf("����д����.....\n\r");
	
	for(i=0;i<256;i++)
	{
		IIC_WritePage(0xA0,i,&buffer[i],1);
		printf("ok\t");
		Delay_ms(50);
	}
	
	printf("д�������\n\r");
	Clear_buffer(buffer,256);
	printf("��������ʼ\n\r");
	IIC_Roundread(0xA0,0,buffer,256);
	printf("���������\n\r");
	
	IIC_Display(buffer); //��ӡ������������
	printf("IIC�������\n\r");
}

/*****************************************************************************************
��������Clear_buffer
�βΣ�
����ֵ����
�������ܣ���һ������ȫ����ֵΪ��0��
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
������:Display_buffer
�βΣ�bufferҪ��ʾ������ĵ�ַ
����ֵ����
�������ܣ����ڴ�ӡһ������
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
������:IIC_Sive
�βΣ���
����ֵ����
�������ܣ�����A��B��C....��ֵ��W24C06
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
	printf("IIC2���Կ�ʼ\n\r");
	printf("��ʼд����\n\r");
	printf("����д����.....\n\r");
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
	printf("д�������\n\r");
	
	
	printf("��������ʼ\n\r");
	IIC_Roundread(0xA0,0,str2,1);
	printf("���������\n\r");
	
	IIC_Display(str2); //��ӡ������������
	printf("IIC�������\n\r");
	
}





































