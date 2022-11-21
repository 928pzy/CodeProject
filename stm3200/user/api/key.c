#include "stm32f10x.h"
#include "key.h"
#include "led.h"


static void Delay(u32 time)
{
	while(time--);
}	

//*************************************************************************************
/*
函数名称:KEY_config
函数功能:按键的初始化
函数参数：无
函数返回值：无
*/
void KEY_config ()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	//初始化GPIO口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//初始化管脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//选择模式
	GPIO_Init(GPIOA,&GPIO_InitStruct);//初始A端口

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3|GPIO_Pin_2;
	GPIO_Init(GPIOE,&GPIO_InitStruct);//初始化E端口为浮空输入
}

//*********************************************************************************
/*
函数名称:Scan_KeyValue
函数功能:获取按键并进行编码
函数参数：无
函数返回值：有
*/
u8 key = 0xff; //不能使key的初始值为0；
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
函数名称:Scan2_KeyValue
函数功能:获取按键并进行编码
函数参数：无
函数返回值：有
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
函数名称:Key
函数功能:一个按键，第一次按下亮，再次按下灭
函数参数：无
函数返回值：有
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
			while (KEY_RIGHT() == 0);//松手检测
		}
	}
}

//********************************************************************************
/*
函数名称:Get_key
函数功能:一个按键，第一次按下亮，再次按下灭
函数参数：无
函数返回值：有
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
函数名称:Get_keyup
函数功能:一个按键，第一次按下亮，再次按下灭(状态机去实现)
函数参数：无
函数返回值：有
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

