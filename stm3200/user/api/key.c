#include "stm32f10x.h"
#include "key.h"
#include "led.h"


static void Delay(u32 time)
{
	while(time--);
}	

//*************************************************************************************
/*
��������:KEY_config
��������:�����ĳ�ʼ��
������������
��������ֵ����
*/
void KEY_config ()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	//��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//��ʼ���ܽ�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ѡ��ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStruct);//��ʼA�˿�

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3|GPIO_Pin_2;
	GPIO_Init(GPIOE,&GPIO_InitStruct);//��ʼ��E�˿�Ϊ��������
}

//*********************************************************************************
/*
��������:Scan_KeyValue
��������:��ȡ���������б���
������������
��������ֵ����
*/
u8 key = 0xff; //����ʹkey�ĳ�ʼֵΪ0��
u8 Scan_KeyValue (void)
{
		if(KEY_UP() == 0)
		{
			Delay(1000);
			if(KEY_UP() == 0)
			{
				key = 1;
			}
		}
		else
		{
			if(KEY_DOWN() == 0)
			{
				Delay(1000);
				if(KEY_DOWN() == 0)
				{
					key = 0;
				}
			}
		}
		return key;
}
	
//**************************************************************************************
/*
��������:Scan2_KeyValue
��������:��ȡ���������б���
������������
��������ֵ����
*/
void Scan2_KeyValue (void)
{
	if (KEY_UP() == 0)
	{
		Delay(1000);
		if(KEY_UP() == 0)
		{
			LED1(1);
		}
	}
	 if (KEY_DOWN() == 0)
	{
		Delay(1000);
		if (KEY_DOWN() == 0)
		{
			LED1(0);
		}
	}
	if (KEY_RIGHT() == 0)
	{
		Delay(1000);
		if (KEY_RIGHT() == 0)
		{
			LED2(1);
		}
	}
	 if (KEY_LEFT() == 1)
	{
		Delay(1000);
		if (KEY_LEFT() == 1)
		{
			LED2(0);
		}
	}
}

//*****************************************************************************************
/*
��������:Key
��������:һ����������һ�ΰ��������ٴΰ�����
������������
��������ֵ����
*/
static int flag = 1;
void Key (void)
{
	if(KEY_RIGHT() == 0)
	{
		Delay(1000);
		if (KEY_RIGHT() == 0)
		{
			flag= !flag;
			LED2(flag);
			while (KEY_RIGHT() == 0);//���ּ��
		}
	}
}

//********************************************************************************
/*
��������:Get_key
��������:һ����������һ�ΰ��������ٴΰ�����
������������
��������ֵ����
*/
#define state1  0
#define state2  1
#define state3  2

u8 Get_key (void)
{
	u8 key = 0xff;
	static u8 state = state1;
		switch (state)
		{
			case state1:
				if (!KEY_UP())
				{
						state = state2;
				}
				break;
				
			case state2:
					Delay(1000);
					if (!KEY_UP())
					{
						state = state3;
						key = 1;
					}
					else 
					{
						state = state1;
					}
					break;
					
			case state3:
					if (KEY_UP())
					{
							state = state1;
					}
					break;
		}
		return key;
}

//***********************************************************************************
/*
��������:Get_keyup
��������:һ����������һ�ΰ��������ٴΰ�����(״̬��ȥʵ��)
������������
��������ֵ����
*/
#define state1  0
#define state2  1
#define state3  2

void Get_keyup (void)
{
	static u32 flag1 = 0;
	static u8 state = state1;

	switch (state)
	{
		case state1:
			if (!KEY_UP())
			{
				state = state2;
			}
		case state2:
			if (!KEY_UP())
			{
				flag1 = ~flag1;
				LED1(flag1);
				state = state3;
			}
			else if (KEY_UP())
			{
				state = state1;
			}
		case state3:
			if (KEY_UP())
			{
				state = state1;
			}
	}
}

//*************************************************************************************
void Get_keydown (void)
{
	static u8 state = state1;
	static u32 flag2 = 0;
	
	switch (state)
	{
		case state1:
			if (!KEY_DOWN())
			{
				state = state2;
			}
		case state2:
			if (!KEY_DOWN())
			{
				flag2 = ~flag2;
				LED2(flag2);
				state = state3;
			}
			else if (KEY_DOWN())
			{
				state = state1;
			}
		case state3:
			if (KEY_DOWN())
			{
				state = state1;
			}
	}
	
}

//**************************************************************************************
	void Get_keyright (void)
	{		
			static u8 state = state1;
			static u32 flag3 = 0;
		
			switch (state)
			{
				case state1:
					if (!KEY_RIGHT())
					{
						state = state2;
					}
				case state2:
					if (!KEY_RIGHT())
					{
							flag3 = ~flag3;
							LED3(flag3);
						state = state3;
						
					}
					else if (KEY_RIGHT())
					{
						state = state1;
					}
				case state3:
					if (KEY_RIGHT())
					{
						state = state1;
					}
			}
		}

//******************************************************************************************
void Get_keylift (void)
{
	static u8 state = state1;
	static u32 flag4 = 0;
	
			switch (state)
			{
				case state1:
					if (KEY_LEFT())
					{
						state = state2;
					}
				case state2:
					if (KEY_LEFT())
					{
						flag4 = ~flag4;
							LED4(flag4);
						state = state3;
				
					}
					else if (!KEY_LEFT())
					{
						state = state1;
					}
				case state3:
					if (!KEY_LEFT())
					{
						state = state1;
					}
			}			
}

