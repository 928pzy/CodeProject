#include "includes.h"

/*
函数：
GPIO_Init();
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

LCD?        STM32

DOUT   MISO   PF8  输入
DIN    MOSI   PF9  输出
PEN    T_PEN  PF10 输入
CS     T_CS   PB2  输出
DCLK   CLK    PB1  输出
*/

/****************************************************
函数名：Delay
形参： 无
返回值：无
函数功能：延时函数
****************************************************/
static void Delay(u32 time)
{    
   u16 i=0;  
   while(time--)
   {
	  i=12000;  //自己定义
	  while(i--) ;    
   }
}
/****************************************************
函数名：TP_GPIOConfig
形参：
返回值：
函数功能：触摸屏初始化
****************************************************/
void TP_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF,ENABLE);
	//输出 PB2 -- T_CS -- CS  PB1 -- CLK -- DCLK  PF9 -- MOSI -- DIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_2;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//输入 PF8 -- MISO -- DOUT PF10 -- T_PEN -- PEN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOF,&GPIO_InitStructure);	
}

/****************************************************
函数名：TP_Read_AD
形参：
返回值：
函数功能：读取触摸屏转化好的AD值
****************************************************/
u16 TP_Read_AD(u8 CMD)
{
	u16 ADNum;
	TCS(0);
	TCLK(0);
	TOUT(0);    //TOUT是神魔含义？？？？
	
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
函数名：TP_Write_Cmd
形参：  CMD要发送的命令
返回值：
函数功能：给触摸屏控制器发送命令
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
函数名：TP_Read_Data
形参：  *AD_Data
返回值：
函数功能：读取触摸屏值
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
函数名：TP_Read_xy
形参：  *x -- x轴坐标
        *y -- y轴坐标
返回值：
函数功能：读取触摸屏x y的坐标值
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
函数名：Test_touch
形参：
返回值：
函数功能：测试触摸屏
****************************************************/
void Test_touch(void)
{
		char buffer_x[10] = {"x=       "};
		char buffer_y[10] = {"y=       "};
		u16 tp_x,tp_y;
		while(TPEN());
		TP_Read_xy(&tp_x,&tp_y);
//		printf("\n\r触摸屏x轴：%d\n\r",tp_x);
//		printf("\n\r触摸屏y轴：%d\n\r",tp_y);
		my_itoa_n(tp_x,buffer_x+2,10);
		my_itoa_n(tp_y,buffer_y+2,10);
		Draw_Text_8_16_Str(20,120,RED,WHITE,(u8 *)buffer_x);
		Draw_Text_8_16_Str(20,160,RED,WHITE,(u8 *)buffer_y);
		//while(!TPEN());
}
/******************************************************************************************************************************************************************/
//用于校准算法的转化
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
函数名：Three_adjust()
形参：
返回值：
函数功能：三点校准
****************************************************/
int A,B,C,D,E,F,K;//校准系数
int num[7];  //定义一个数组保存校准的系数
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
	
	//串口打印A B C D E F K的系数
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
	
	if(Adjust_test(120,160) == 0)//如果不正确，请重新校准
		goto loop1;
	
}
/****************************************************
函数名：Adjust_test
形参：  x -- 检测的x坐标
        y -- 检测的y坐标
返回值：
函数功能：检测校准是否正确
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
函数名：Display_LCD_xy
形参：
返回值：
函数功能：在LCD屏上显示坐标值
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
函数名：Get_LCDXYValueFormTP
形参：
返回值：
函数功能：获得触摸屏坐标值，并转化为LCD屏坐标值
****************************************************/
//触摸屏的三种状态
#define TP_NoPressState  0   //触摸屏准备被按下
#define	TP_PressingState 1   //触摸屏正在被按下  
#define TP_PressedState  2   //触摸屏被松开 
static int lcd_xy[2];//保存获得的LCD屏坐标值             
int *Get_LCDXYValueFormTP(void)
{
	u16 tp_x,tp_y;       //保存获得的触摸屏坐标值
	static u8 tp_state = TP_NoPressState;//设置触摸屏当前状态
	
	switch(tp_state)//根据TPEN()管脚电平修改触摸屏现在所处的状态
	{
		case TP_NoPressState:
			 if(!TPEN())//查看触摸屏是否被按下，如果按下，把tp_state修改为TP_PressingState（代表正在被按下）
			 {
				tp_state = TP_PressingState;
				TP_Read_xy(&tp_x,&tp_y);
				lcd_xy[0] = (A*tp_x+B*tp_y+C)/K;
				lcd_xy[1] = (D*tp_x+E*tp_y+F)/K;
				return lcd_xy;
			 }
			 break;
		case TP_PressingState:
		     if(!TPEN())//查看触摸屏是否被按下，如果按下，把tp_state修改为TP_PressedState，并返回该按键的返回值
			 {
				tp_state = TP_PressedState;
			 }
			 else //按键被松开
				tp_state = TP_NoPressState;
			break;
		case TP_PressedState:
			 if(TPEN())
				 tp_state = TP_NoPressState;break;
	}
	return 0;

}
/****************************************************
函数名：my_itoa_n
形参：  num(要转化的数)
        str(转化好的数组的地址)
        radix(进制 2 8 10 16)
返回值：
函数功能：把数字转化为字符串
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
函数名：IIC_WritePage_Adjust
形参：  无
返回值：无
函数功能：把三点校准的系数值保存到W24C06中
****************************************************/
void IIC_WritePage_Adjust (void)
{
		int i;
	union INTTOCHAR      //定义一个共用体
		{
			int buffer[7];
			char str[28];
		};
		union INTTOCHAR temp1;
		u8 adjust_flag = 0;
pp1:
		Three_adjust();      //先校准
		for (i = 0;i < 7;i++) //把校准的系数存储在共用体数组中
		{
			temp1.buffer[i] = num[i];
		}
		for (i = 8;i < 36;i++)      //把"ABCDEFK"写入芯片中，第一页留给标志位，7个数需写4页
		{
				IIC_WritePage(0xA0,i,&temp1.str[i-8],1);//从第2页开始写，写到第5页
				Delay_ms(50);
				printf("ok\n");
		}
		adjust_flag = 0x01;  //设置标志位
		IIC_WritePage(0xA0,0,&adjust_flag,1); //把标志位写入芯片
		Delay_ms(50);
/****************************************************************************************************/
		printf("adjust will read!!!!!!");
		IIC_Roundread(0xA0,0,&adjust_flag,1);//读取标志位
		printf("adjust is reading!!!!!!");
		if (adjust_flag != 0x01)
				goto pp1;
}

/****************************************************
函数名：IIC_ReadPage_Adjust
形参：  无
返回值：无
函数功能：从芯片中读取三点校准的系数值
****************************************************/
void IIC_ReadPage_Adjust(void)
{
	u8 adjust_flag;
	union STUDENT      //定义共用体
		{
			int buffer[7];
			u8 str[28];
		};
		union STUDENT ptemp;
pp1:		
		IIC_Roundread(0xA0,0,&adjust_flag,1);//读取标志位
		if(adjust_flag==0x01)
		{
			IIC_Roundread(0xA0,8,ptemp.str,28);
		}
		else 
		{
			IIC_WritePage_Adjust();			//进行三点校验并保存校验系数
			goto pp1;
		}
		
		A = ptemp.buffer[0];
		B = ptemp.buffer[1];
		C = ptemp.buffer[2];
		D = ptemp.buffer[3];
		E = ptemp.buffer[4];
		F = ptemp.buffer[5];
		K = ptemp.buffer[6];
		
	//串口打印A B C D E F K的系数
//	printf("\n\rA=%d\n\r",A);
//	printf("\n\rB=%d\n\r",B);
//	printf("\n\rC=%d\n\r",C);
//	printf("\n\rD=%d\n\r",D);
//	printf("\n\rE=%d\n\r",E);
//	printf("\n\rF=%d\n\r",F);
//	printf("\n\rK=%d\n\r",K);
}












