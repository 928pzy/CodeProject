#include "includes.h"

/*
������
GPIO_Init();
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

LCD?        STM32

DOUT   MISO   PF8  ����
DIN    MOSI   PF9  ���
PEN    T_PEN  PF10 ����
CS     T_CS   PB2  ���
DCLK   CLK    PB1  ���
*/

/****************************************************
��������Delay
�βΣ� ��
����ֵ����
�������ܣ���ʱ����
****************************************************/
static void Delay(u32 time)
{    
   u16 i=0;  
   while(time--)
   {
	  i=12000;  //�Լ�����
	  while(i--) ;    
   }
}
/****************************************************
��������TP_GPIOConfig
�βΣ�
����ֵ��
�������ܣ���������ʼ��
****************************************************/
void TP_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF,ENABLE);
	//��� PB2 -- T_CS -- CS  PB1 -- CLK -- DCLK  PF9 -- MOSI -- DIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_2;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//���� PF8 -- MISO -- DOUT PF10 -- T_PEN -- PEN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOF,&GPIO_InitStructure);	
}

/****************************************************
��������TP_Read_AD
�βΣ�
����ֵ��
�������ܣ���ȡ������ת���õ�ADֵ
****************************************************/
u16 TP_Read_AD(u8 CMD)
{
	u16 ADNum;
	TCS(0);
	TCLK(0);
	TOUT(0);    //TOUT����ħ���壿������
	
	TP_Write_Cmd(CMD);
	Delay_us(10);
	
	TCLK(0);
	Delay_us(1);
	TCLK(1);
	
	TP_Read_Data(&ADNum);
	ADNum >>= 4;
	
	TCS(1);
	return ADNum;
}

/****************************************************
��������TP_Write_Cmd
�βΣ�  CMDҪ���͵�����
����ֵ��
�������ܣ�����������������������
****************************************************/
void TP_Write_Cmd(u8 CMD)
{
	u16 counter = 0;
	u8 CMD_Mask = 0x80;
	TCLK(0);
	for(counter=0;counter<8;counter++)
	{
		if(CMD & CMD_Mask) TOUT(1);
		else TOUT(0);
		TCLK(1);
		TCLK(0);
		CMD <<= 1;
	}
	
}
/****************************************************
��������TP_Read_Data
�βΣ�  *AD_Data
����ֵ��
�������ܣ���ȡ������ֵ
****************************************************/
void TP_Read_Data(u16 *AD_Data)
{
	u16 counter;
	for(counter = 0;counter<16;counter++)
	{
		TCLK(0);
		TCLK(1);
		(*AD_Data)<<=1;
		if(TDIN()) (*AD_Data)++;
	}
}
/****************************************************
��������TP_Read_xy
�βΣ�  *x -- x������
        *y -- y������
����ֵ��
�������ܣ���ȡ������x y������ֵ
****************************************************/
void TP_Read_xy(u16 *x,u16 *y)
{
	u16 temp_x,temp_y;
	temp_x = TP_Read_AD(CMD_RDX);
	temp_y = TP_Read_AD(CMD_RDY);
	
	*x = temp_x;
	*y = temp_y;
}
/****************************************************
��������Test_touch
�βΣ�
����ֵ��
�������ܣ����Դ�����
****************************************************/
void Test_touch(void)
{
		char buffer_x[10] = {"x=       "};
		char buffer_y[10] = {"y=       "};
		u16 tp_x,tp_y;
		while(TPEN());
		TP_Read_xy(&tp_x,&tp_y);
//		printf("\n\r������x�᣺%d\n\r",tp_x);
//		printf("\n\r������y�᣺%d\n\r",tp_y);
		my_itoa_n(tp_x,buffer_x+2,10);
		my_itoa_n(tp_y,buffer_y+2,10);
		Draw_Text_8_16_Str(20,120,RED,WHITE,(u8 *)buffer_x);
		Draw_Text_8_16_Str(20,160,RED,WHITE,(u8 *)buffer_y);
		//while(!TPEN());
}
/******************************************************************************************************************************************************************/
//����У׼�㷨��ת��
#define XL1 LCD_X[0]
#define XL2 LCD_X[1]
#define XL3 LCD_X[2]

#define YL1 LCD_Y[0]
#define YL2 LCD_Y[1]
#define YL3 LCD_Y[2]

#define X1  TP_X[0]
#define X2  TP_X[1]
#define X3  TP_X[2]

#define Y1  TP_Y[0]
#define Y2  TP_Y[1]
#define Y3  TP_Y[2]
/****************************************************
��������Three_adjust()
�βΣ�
����ֵ��
�������ܣ�����У׼
****************************************************/
int A,B,C,D,E,F,K;//У׼ϵ��
int num[7];  //����һ�����鱣��У׼��ϵ��
void Three_adjust(void)
{
	u32 LCD_X[3] = {50,180,40};
	u32 LCD_Y[3] = {50,160,280};
	u16 TP_X[3],TP_Y[3];
	u32 tp_num = 0;
	int i;
loop1:	
	for(tp_num = 0;tp_num<3;tp_num++)
	{
		for(i=0;i<5;i++)
		{
			LCD_DrawPoint(LCD_X[tp_num]+i,LCD_Y[tp_num],RED);
			LCD_DrawPoint(LCD_X[tp_num]-i,LCD_Y[tp_num],RED);
			LCD_DrawPoint(LCD_X[tp_num],LCD_Y[tp_num]+i,RED);
			LCD_DrawPoint(LCD_X[tp_num],LCD_Y[tp_num]-i,RED);
		}
		while(TPEN() != 0);
		TP_Read_xy(&TP_X[tp_num],&TP_Y[tp_num]);
		Delay(300);
		while(TPEN() == 0);
	}
	
	K = (X1 - X3)*(Y2 - Y3) - (X2 - X3)*(Y1 - Y3);
	A = ((XL1 - XL3)*(Y2 - Y3) - (XL2 - XL3)*(Y1 - Y3));
	B = (( X1 - X3 )*( XL2 - XL3) - (XL1 - XL3)*( X2 - X3));
	C = (Y1*( X3*XL2 - X2*XL3) + Y2*(X1*XL3 - X3*XL1) + Y3*(X2*XL1 - X1*XL2));
	D = ((YL1 - YL3)*(Y2 - Y3) - (YL2 - YL3)*(Y1 - Y3));
	E = ((X1 - X3)*(YL2 - YL3) - (YL1 - YL3)*(X2 - X3));
	F = (Y1*(X3*YL2 - X2*YL3) + Y2*(X1*YL3 - X3*YL1) + Y3*(X2*YL1 - X1*YL2));
	
	//���ڴ�ӡA B C D E F K��ϵ��
//	printf("\n\rA=%d\n\r",A);
//	printf("\n\rB=%d\n\r",B);
//	printf("\n\rC=%d\n\r",C);
//	printf("\n\rD=%d\n\r",D);
//	printf("\n\rE=%d\n\r",E);
//	printf("\n\rF=%d\n\r",F);
//	printf("\n\rK=%d\n\r",K);
	num[0] = A;
	num[1] = B;
	num[2] = C;
	num[3] = D;
	num[4] = E;
	num[5] = F;
	num[6] = K;
	
	if(Adjust_test(120,160) == 0)//�������ȷ��������У׼
		goto loop1;
	
}
/****************************************************
��������Adjust_test
�βΣ�  x -- ����x����
        y -- ����y����
����ֵ��
�������ܣ����У׼�Ƿ���ȷ
****************************************************/
u8 Adjust_test(u16 x,u16 y)
{
	u32 i;
	u16 tp_x,tp_y;
	int lcd_x,lcd_y;
	for(i=0;i<10;i++)
	{
		LCD_DrawPoint(x+i,y,GRED);
		LCD_DrawPoint(x-i,y,GRED);
		LCD_DrawPoint(x,y+i,GRED);
		LCD_DrawPoint(x,y-i,GRED);
	}
	while(TPEN());
	TP_Read_xy(&tp_x,&tp_y);
//	printf("\n\rtp_x = %d\n\r",tp_x);
//	printf("\n\rtp_y = %d\n\r",tp_y);
	lcd_x = (A*tp_x+B*tp_y+C)/K;
	lcd_y = (D*tp_x+E*tp_y+F)/K;
//	printf("\n\rlcd_x = %d \n\rlcd_y = %d\n\r\n\r",lcd_x,lcd_y);
	if(lcd_x < x+10 && lcd_x>x-10  && lcd_y > y-10 && lcd_y<y+10)
	{
		Draw_Text_8_16_Str(80,240,RED,WHITE,"Adjust OK");
		Delay_ms(2000);
		LCD_Clear(BLUE);
		return 1;
	}
	else
	{
		while(!TPEN());
		Draw_Text_8_16_Str(20,240,RED,WHITE,"Adjust error,Place again");
		Delay_ms(2000);
		
		LCD_Clear(BLUE);
		return 0;
	}
}
/****************************************************
��������Display_LCD_xy
�βΣ�
����ֵ��
�������ܣ���LCD������ʾ����ֵ
****************************************************/
void Display_LCD_xy(void)
{
		int lcd_x,lcd_y;
		char buffer_x[10] = {"x=     "};
		char buffer_y[10] = {"y=     "};
		u16 tp_x,tp_y;
		if(!TPEN())
		{
			TP_Read_xy(&tp_x,&tp_y);
			lcd_x = (A*tp_x+B*tp_y+C)/K;
			lcd_y = (D*tp_x+E*tp_y+F)/K;
			my_itoa_n(lcd_x,buffer_x+2,10);
			my_itoa_n(lcd_y,buffer_y+2,10);
			Draw_Text_8_16_Str(20,240,RED,WHITE,(u8 *)buffer_x);
			Draw_Text_8_16_Str(20,260,RED,WHITE,(u8 *)buffer_y);
		}
}

/****************************************************
��������Get_LCDXYValueFormTP
�βΣ�
����ֵ��
�������ܣ���ô���������ֵ����ת��ΪLCD������ֵ
****************************************************/
//������������״̬
#define TP_NoPressState  0   //������׼��������
#define	TP_PressingState 1   //���������ڱ�����  
#define TP_PressedState  2   //���������ɿ� 
static int lcd_xy[2];//�����õ�LCD������ֵ             
int *Get_LCDXYValueFormTP(void)
{
	u16 tp_x,tp_y;       //�����õĴ���������ֵ
	static u8 tp_state = TP_NoPressState;//���ô�������ǰ״̬
	
	switch(tp_state)//����TPEN()�ܽŵ�ƽ�޸Ĵ���������������״̬
	{
		case TP_NoPressState:
			 if(!TPEN())//�鿴�������Ƿ񱻰��£�������£���tp_state�޸�ΪTP_PressingState���������ڱ����£�
			 {
				tp_state = TP_PressingState;
				TP_Read_xy(&tp_x,&tp_y);
				lcd_xy[0] = (A*tp_x+B*tp_y+C)/K;
				lcd_xy[1] = (D*tp_x+E*tp_y+F)/K;
				return lcd_xy;
			 }
			 break;
		case TP_PressingState:
		     if(!TPEN())//�鿴�������Ƿ񱻰��£�������£���tp_state�޸�ΪTP_PressedState�������ظð����ķ���ֵ
			 {
				tp_state = TP_PressedState;
			 }
			 else //�������ɿ�
				tp_state = TP_NoPressState;
			break;
		case TP_PressedState:
			 if(TPEN())
				 tp_state = TP_NoPressState;break;
	}
	return 0;

}
/****************************************************
��������my_itoa_n
�βΣ�  num(Ҫת������)
        str(ת���õ�����ĵ�ַ)
        radix(���� 2 8 10 16)
����ֵ��
�������ܣ�������ת��Ϊ�ַ���
****************************************************/
char *my_itoa_n(int num,char *str,int radix)
{
	const char table[] = "0123456789ABCDEF";
	char *ptr = str;
	char *start = NULL;
    char temp;
	char negative = 0;
	if(num == 0)
	{
		*ptr++ = '0';
//		*ptr='\0';
		return str;
	}
	if(num<0)
	{
		*ptr++='-';
		num = num*(-1);
		negative = 1;
	}
	while(num)
	{
		*ptr++ = table[num % radix];
		num/=radix;
	}
//	*ptr = '\0';
	start = (negative ? str+1:str);
	ptr--;
	while(start<ptr)
	{
		temp = *start;
		*start = *ptr;
		*ptr = temp;
		start++;
		ptr--;
	}
	return str;
}

/****************************************************
��������IIC_WritePage_Adjust
�βΣ�  ��
����ֵ����
�������ܣ�������У׼��ϵ��ֵ���浽W24C06��
****************************************************/
void IIC_WritePage_Adjust (void)
{
		int i;
	union INTTOCHAR      //����һ��������
		{
			int buffer[7];
			char str[28];
		};
		union INTTOCHAR temp1;
		u8 adjust_flag = 0;
pp1:
		Three_adjust();      //��У׼
		for (i = 0;i < 7;i++) //��У׼��ϵ���洢�ڹ�����������
		{
			temp1.buffer[i] = num[i];
		}
		for (i = 8;i < 36;i++)      //��"ABCDEFK"д��оƬ�У���һҳ������־λ��7������д4ҳ
		{
				IIC_WritePage(0xA0,i,&temp1.str[i-8],1);//�ӵ�2ҳ��ʼд��д����5ҳ
				Delay_ms(50);
				printf("ok\n");
		}
		adjust_flag = 0x01;  //���ñ�־λ
		IIC_WritePage(0xA0,0,&adjust_flag,1); //�ѱ�־λд��оƬ
		Delay_ms(50);
/****************************************************************************************************/
		printf("adjust will read!!!!!!");
		IIC_Roundread(0xA0,0,&adjust_flag,1);//��ȡ��־λ
		printf("adjust is reading!!!!!!");
		if (adjust_flag != 0x01)
				goto pp1;
}

/****************************************************
��������IIC_ReadPage_Adjust
�βΣ�  ��
����ֵ����
�������ܣ���оƬ�ж�ȡ����У׼��ϵ��ֵ
****************************************************/
void IIC_ReadPage_Adjust(void)
{
	u8 adjust_flag;
	union STUDENT      //���干����
		{
			int buffer[7];
			u8 str[28];
		};
		union STUDENT ptemp;
pp1:		
		IIC_Roundread(0xA0,0,&adjust_flag,1);//��ȡ��־λ
		if(adjust_flag==0x01)
		{
			IIC_Roundread(0xA0,8,ptemp.str,28);
		}
		else 
		{
			IIC_WritePage_Adjust();			//��������У�鲢����У��ϵ��
			goto pp1;
		}
		
		A = ptemp.buffer[0];
		B = ptemp.buffer[1];
		C = ptemp.buffer[2];
		D = ptemp.buffer[3];
		E = ptemp.buffer[4];
		F = ptemp.buffer[5];
		K = ptemp.buffer[6];
		
	//���ڴ�ӡA B C D E F K��ϵ��
//	printf("\n\rA=%d\n\r",A);
//	printf("\n\rB=%d\n\r",B);
//	printf("\n\rC=%d\n\r",C);
//	printf("\n\rD=%d\n\r",D);
//	printf("\n\rE=%d\n\r",E);
//	printf("\n\rF=%d\n\r",F);
//	printf("\n\rK=%d\n\r",K);
}












