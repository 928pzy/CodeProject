#include "includes.h"

/*********************************************************************************
��������:TIM6_Config
��������:TIM6��ʱ���ĳ�ʼ��
����������prescaler -- ��Ƶֵ
					period -- ��װ��ֵ
��������ֵ����
**********************************************************************************/
void TIM6_Config (u16 prescaler,u16 period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		//��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	//TIM6�ĳ�ʼ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;   //������ݼ�ģʽ
	TIM_TimeBaseInitStruct.TIM_Period = period;  //��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;  //��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);   //ʹ��TIM�ж�
	TIM_Cmd(TIM6,ENABLE);  	//ʹ��TIM����
	TIM6->DIER &=0<<0;
}	

/*********************************************************************************
��������:NVIC_Config
��������:��ʼ���ж�ͨ��
������������
��������ֵ����
**********************************************************************************/
void NVIC6_Config (void)
{
		NVIC_InitTypeDef NVIC_InitStruct;
		//��ʼ��NVIC
		NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;//�ж�EXTI0ͨ��
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//ѡ��ռ�����ȼ��ĵȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//ѡ��μ����ȼ��ĵȼ�
		NVIC_Init(&NVIC_InitStruct);	
}

/*********************************************************************************
��������:TIM6_IRQHandler 
��������:�жϷ�����
������������
��������ֵ����
**********************************************************************************/
u32 sum = 0;
void TIM6_IRQHandler (void)
{
	if (TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
			sum++;
	}
}

/*********************************************************************************
��������:TIM6_Seconds
��������:�ö�ʱ����ʵ�����
������������
��������ֵ����
**********************************************************************************/
uint32_t TM = 0,TS = 0,TMS = 0;
void TIM6_Seconds(void)
{
	char buffer_mm[3];
	char buffer_ss[3];
	char buffer_ms[3];
	TMS = TMS + 1;
	if (TMS == 100)
	{
			TS = TS + 1; 
			TMS = 0;
	}
	if (TS == 60)
	{
			TM = TM + 1;
			TS = 0;
	}
	
	my_itoa_n(TS,buffer_ss,10);
	my_itoa_n(TM,buffer_mm,10);
	my_itoa_n(TMS,buffer_ms,10);
	
	LCD_Clear(WHITE);
	
	Draw_Text_8_16_Str(140,290,WHITE,BLACK,(u8 *)buffer_ms);
	Draw_Text_8_16_Str(120,290,WHITE,BLACK,(u8 *)buffer_ss);
	Draw_Text_8_16_Str(100,290,WHITE,BLACK,(u8 *)buffer_mm);
}

/*********************************************************************************
��������:TIM6_Seconds_2
��������:�ö�ʱ����ʵ�����
������������
��������ֵ����
**********************************************************************************/
//*****************************************************************************************************
u8 LCD_Watch_Num[][16]=
{
	/*--  ����:  0  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,},
		
		/*--  ����:  1  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,},
		
		/*--  ����:  2  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00,},
		
		/*--  ����:  3  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00,},
		
		/*--  ����:  4  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00,},
		
		/*--  ����:  5  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,},
		
		/*--  ����:  6  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00,},
		
		/*--  ����:  7  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,},
		
		/*--  ����:  8  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00,},
		
		/*--  ����:  9  --*/
		/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
		{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x26,0x1A,0x02,0x02,0x24,0x38,0x00,0x00,}
};

void TIM6_Seconds_2 (void)
{
	u8 str_ms[2];
	u8 str_ss[2];
	u8 str_mm[2];
	
	str_mm[0] = sum/6000/10;        
	str_mm[1] = sum/6000;
	
	str_ss[0] = sum%6000/100/10;
	str_ss[1] = sum%6000/100%10;
	
	str_ms[0] = sum%6000%100/10;
	str_ms[1] = sum%6000%100%10;
	
	show_xXx(100,300,BLACK,WHITE,LCD_Watch_Num[str_mm[0]],16,8);
	show_xXx(110,300,BLACK,WHITE,LCD_Watch_Num[str_mm[1]],16,8);
	
	show_xXx(120,300,BLACK,WHITE,LCD_Watch_Num[str_ss[0]],16,8);
	show_xXx(130,300,BLACK,WHITE,LCD_Watch_Num[str_ss[1]],16,8);
	
	show_xXx(140,300,BLACK,WHITE,LCD_Watch_Num[str_ms[0]],16,8);
	show_xXx(150,300,BLACK,WHITE,LCD_Watch_Num[str_ms[1]],16,8);
} 

/*********************************************************************************
��������:TIM6_Seconds_Counter
��������:����Ļ������ʵ�ֿ������
������������
��������ֵ����
**********************************************************************************/
void TIM6_Seconds_Counter(void)
{
		static u32 TIME_flag = 0;
		int lcd_x,lcd_y;
		u16 tp_x,tp_y;
		if(!TPEN())         //�ж���Ļ�Ƿ񱻰���
		{
			Delay_ms(10);
			if(!TPEN())
			{
				TP_Read_xy(&tp_x,&tp_y);
				lcd_x = (A*tp_x+B*tp_y+C)/K;
				lcd_y = (D*tp_x+E*tp_y+F)/K;
			}
		}
//***�����ǻ�ȡLCD����,������ͨ���������������***************************************
		if ((lcd_x<=110) && (lcd_x>=80) && (lcd_y>=240) && (lcd_y<=280))
			{
				sum = 0;
			}		
		if ((lcd_x<=160) && (lcd_x>=130) && (lcd_y>=240) && (lcd_y<=280))
			{
					TIME_flag = !TIME_flag;
					if (TIME_flag == 0)
					{
							TIM6->DIER &=0<<0;
					}
					else if (TIME_flag == 1)
					{
							TIM6->DIER |=1<<0;
					}
			}
}

