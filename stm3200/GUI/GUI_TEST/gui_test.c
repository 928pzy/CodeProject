
#include "includes.h"

void GUI_TEST(void)
{
	GUI_Initialize();//LCD屏初始化
	GUI_SetColor(BLACK,WHITE);
//	GUI_DrewLinetest();
//	GUI_DrewBasetest();
//	GUI_DrewWindowstest();
//	GUI_DrewLoadbit();
//	GUI_DrewFont5_7();
//	GUI_DrewFont8_8();
//	GUI_DrewFont24_32();
//	GUI_DrewMenustest();
//	GUI_DrewSpline();
	
}

void GUI_DrewLinetest(void)//用于LCDDRIVE.c
{
	//画水平线
	GUI_HLine(20,20,100,WHITE);
	//画垂直线
	GUI_RLine(20,20,100,WHITE);
}

uint32  const poly6[] = {150,110, 170,80, 180,95, 200,85, 230,110, 150,110};					
void GUI_DrewBasetest(void)
{
	//画任意两点之间的直线
	GUI_Line(0,0,240,320,WHITE);
	//画具有线宽的两点之间的直线 线宽最大为50
	GUI_LineWith(240,0,0,320,10,WHITE);
	//画多边形
	GUI_LineS(poly6,6,WHITE);
	//画矩形
	GUI_Rectangle(10,10,60,70,WHITE);
	//画矩形填充颜色
	GUI_RectangleFill(10,80,60,130,WHITE);
	//画正方型
	GUI_Square(10,140,40,WHITE);
	//画圆
	GUI_Circle(120,50,40,RED);
	//画填充颜色的圆
	GUI_CircleFill(120,270,40,YELLOW);
	//画椭圆
	GUI_Ellipse(120,200,130,180,WHITE);
	//画填充颜色的椭圆
	GUI_EllipseFill(150,230,200,250,WHITE);
	//图形填充
	//GUI_FloodFill(160,100,BLUE);//读取LCD屏颜色值，所以这个函数不行
	//1/4圆弧
	GUI_Arc4(20,270,50,4,RED);
	//任意角度圆弧
	GUI_Arc(20,270,40,330,270,GREEN);//角度刚好相反 --- 逆时针画圆
	//扇形
	GUI_Pieslice(20,270,30,330,270,BLUE);//角度刚好相反 --- 逆时针画圆
}
WINDOWS window_test;
void GUI_DrewWindowstest(void)
{
	window_test.x = 0;
	window_test.y = 0;
	window_test.with = 240;
	window_test.hight = 320;
	window_test.title = "STM32 GUI_TEST";
	window_test.state = "www.edu118.com";
	//画窗口函数
	GUI_WindowsDraw(&window_test);
//	Delay_ms(2000);
////	//隐藏窗口函数
//	GUI_WindowsHide(&window_test);
//	Delay_ms(2000);
//	//画窗口函数
//	GUI_WindowsDraw(&window_test);
//	GUI_Line(120,30,120,290,RED);
//	Delay_ms(2000);
//	//清窗口显示内容
//	GUI_WindowsClr(&window_test);
}
char *menustr[] = {"File","Edit","View"};
char *senustr[] = {"New","Open","Close"}; 
MENUICO menuicotest;
MMENU mmenu;
SMENU smenu;
void senutest(void)
{
	Draw_Text_8_16_Str(60,220,RED,WHITE,"Welcome STM32");
}
void GUI_DrewMenustest(void)
{
//	//GUI_Button49x14();
	GUI_Button_OK(10,100);
	GUI_Button_OK1(110,100);
	GUI_Button_Cancle(60,100);
	GUI_Button_Cancle1(160,100);
//	GUI_MenuIcoDraw();
	mmenu.win = &window_test;
	mmenu.no = 3;
	mmenu.str[0] = menustr[0];
	mmenu.str[1] = menustr[1];
	mmenu.str[2] = menustr[2];
	GUI_MMenuDraw(&mmenu);
	
	GUI_MMenuSelect(&mmenu,0);
	Delay_ms(1000);
	GUI_MMenuNSelect(&mmenu,0);
	
	smenu.win = &window_test;
	smenu.mmenu_no = 0;
	smenu.no = 3;
	smenu.str[0] = senustr[0];
	smenu.str[1] = senustr[1];
	smenu.str[2] = senustr[2];
	smenu.state = 0;
	smenu.Function[2] =  senutest;
	
	GUI_SMenuDraw(&smenu);
	Delay_ms(500);
	GUI_SMenuSelect(&smenu,0,1);
	Delay_ms(500);
	GUI_SMenuSelect(&smenu,1,2);
	//执行回调函数
	smenu.Function[smenu.state]();
	Delay_ms(1000);
	
	GUI_SMenuHide(&smenu);
	
}

//单色笑脸图片
uint8 const ICO1[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x07,0x00,0xE0,0x00,0x00,0x18,
0x00,0x18,0x00,0x00,0x30,0x00,0x0C,0x00,0x00,0x40,0x00,0x02,0x00,0x00,0x80,0x00,
0x01,0x00,0x01,0x00,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0xC0,0x02,0x01,0x81,0x80,
0x40,0x04,0x03,0xC3,0xC0,0x20,0x04,0x03,0xC3,0xC0,0x20,0x04,0x03,0xC3,0xC0,0x20,
0x08,0x03,0xC3,0xC0,0x10,0x08,0x03,0xC3,0xC0,0x10,0x08,0x01,0x81,0x80,0x10,0x08,
0x08,0x00,0x10,0x10,0x08,0x08,0x00,0x10,0x10,0x08,0x10,0x00,0x08,0x10,0x08,0xF0,
0x00,0x0F,0x10,0x08,0x08,0x00,0x10,0x10,0x04,0x04,0x00,0x20,0x20,0x04,0x06,0x00,
0x60,0x20,0x04,0x03,0x81,0xC0,0x20,0x02,0x01,0x7E,0x80,0x40,0x03,0x00,0x81,0x00,
0xC0,0x01,0x00,0x42,0x00,0x80,0x00,0x80,0x3C,0x01,0x00,0x00,0x40,0x00,0x02,0x00,
0x00,0x30,0x00,0x0C,0x00,0x00,0x18,0x00,0x18,0x00,0x00,0x07,0x00,0xE0,0x00,0x00,
0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
//汉字龍 56*56
/*--  ??:  ?  --*/
/*--  ??42;  ??????????:?x?=56x56   --*/
u8 const HZ_long[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,
0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x03,0xC0,0x00,0x00,0x00,0x0F,0x80,0x03,
0xF0,0x00,0x00,0x00,0x07,0xC0,0x03,0xE0,0x00,0x00,0x00,0x07,0xC0,0x03,0xC0,0x00,
0x00,0x00,0x03,0xC0,0x03,0xC0,0x0E,0x00,0x00,0x03,0xC3,0x83,0xC0,0x1F,0x00,0x00,
0x03,0x87,0xC3,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0xE3,0xC0,0x00,0xC0,0x07,0xFF,0xFF,
0xF3,0xC0,0x00,0x00,0x01,0x80,0x1E,0x03,0xC0,0x00,0x00,0x01,0xE0,0x3F,0x03,0xC0,
0x0C,0x00,0x00,0xF8,0x3E,0x03,0xC0,0x0E,0x00,0x00,0x7C,0x3C,0x07,0xFF,0xFF,0x80,
0x00,0x3C,0x78,0x07,0xFF,0xFF,0x80,0x00,0x3E,0x78,0x03,0xC0,0x1F,0x00,0x00,0x3C,
0x70,0x00,0x00,0x1F,0x00,0x00,0x18,0xE1,0xE0,0x00,0x1F,0x00,0x00,0x00,0xC3,0xF0,
0x00,0x1F,0x00,0x3F,0xFF,0xFF,0xFF,0x00,0x1F,0x00,0x1E,0x00,0x00,0x1F,0x80,0x1F,
0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0x00,0x01,0x80,0x06,0x07,0xC0,0x1F,0x00,0x01,
0xE0,0x0F,0x87,0xC0,0x18,0x00,0x01,0xFF,0xFF,0xC7,0xC0,0x00,0x00,0x01,0xE0,0x0F,
0xC7,0xC0,0x18,0x00,0x01,0xE0,0x0F,0x07,0xC0,0x3C,0x00,0x01,0xE0,0x0F,0x07,0xFF,
0xFE,0x00,0x01,0xE0,0x0F,0x07,0xFF,0xFF,0x00,0x01,0xE0,0x0F,0x07,0xC0,0x00,0x00,
0x01,0xFF,0xFF,0x07,0xC0,0x00,0x00,0x01,0xFF,0xFF,0x07,0xC0,0x18,0x00,0x01,0xE0,
0x0F,0x07,0xC0,0x3C,0x00,0x01,0xE0,0x0F,0x07,0xFF,0xFE,0x00,0x01,0xE0,0x0F,0x07,
0xFF,0xFE,0x00,0x01,0xE0,0x0F,0x07,0xC0,0x00,0x00,0x01,0xE0,0x0F,0x07,0xC0,0x00,
0x00,0x01,0xFF,0xFF,0x07,0xC0,0x00,0x00,0x01,0xE0,0x0F,0x07,0xC0,0x3C,0xE0,0x01,
0xE0,0x0F,0x07,0xFF,0xFE,0xE0,0x01,0xE0,0x0F,0x07,0xFF,0xFF,0xE0,0x01,0xE0,0x0F,
0x07,0xC0,0x00,0xE0,0x01,0xE0,0x0F,0x07,0xC0,0x00,0xE0,0x01,0xE0,0x0F,0x07,0xC0,
0x00,0xE0,0x01,0xE0,0x0F,0x07,0xC0,0x00,0xE0,0x01,0xE0,0x0F,0x07,0xC0,0x00,0xF0,
0x01,0xE0,0x0F,0x07,0xC0,0x00,0xF0,0x01,0xE1,0xFF,0x03,0xE0,0x01,0xF8,0x01,0xE0,
0x7F,0x03,0xFF,0xFF,0xF8,0x01,0xE0,0x3E,0x01,0xFF,0xFF,0xF0,0x01,0xC0,0x1C,0x00,
0x7F,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void GUI_DrewLoadbit(void)
{
	GUI_LoadPic(50,50,(uint8 *)ICO1,40,40);
	GUI_LoadPic1(150,50,(uint8 *)ICO1,40,40);
	
	GUI_PutHZ(90,150,(u8 *)HZ_long,56,56);
}

void GUI_DrewFont5_7(void)
{
	GUI_PutChar(10,10,'E');
	GUI_PutString(10,20,"STM32 GUI");
	GUI_PutNoStr(10,30,"www.edu118.com",10);
}
void GUI_DrewFont8_8(void)
{
	GUI_PutChar8_8(80,10,'E');
	GUI_PutString8_8(80,20,"STM32 GUI");
	GUI_PutNoStr8_8(80,30,"www.edu118.com",10);
}
void GUI_DrewFont24_32(void)
{
	GUI_PutChar24_32(10,80,'1');
}
//画曲线用到的坐标
PointXY log_pin[] = { 
					  {80,  30},
                      {120, 60},
                      {160, 30},
                      {200, 60}
                    };
void GUI_DrewSpline(void)
{
	GUI_Spline(log_pin, 4, WHITE); 
}


