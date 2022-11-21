#ifndef _TOUCH_H_
#define _TOUCH_H_
#include "stm32f10x.h"
/*
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
*/
#define TPEN()  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)
#define TDIN()  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)

#define TCS(x)  (x)?GPIO_SetBits(GPIOB,GPIO_Pin_2):GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define TCLK(x) (x)?GPIO_SetBits(GPIOB,GPIO_Pin_1):GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define TOUT(x) (x)?GPIO_SetBits(GPIOF,GPIO_Pin_9):GPIO_ResetBits(GPIOF,GPIO_Pin_9)

#define CMD_RDX 0xD0
#define CMD_RDY 0x90
extern int A,B,C,D,E,F,K;

void TP_GPIOConfig(void);
void TP_Write_Cmd(u8 CMD);
void TP_Read_Data(u16 *AD_Data);
u16 TP_Read_AD(u8 CMD);
void TP_Read_xy(u16 *x,u16 *y);
void Three_adjust(void);
int *Get_LCDXYValueFormTP(void);


void Test_touch(void);
u8 Adjust_test(u16 x,u16 y);
void Display_LCD_xy(void);
char *my_itoa_n(int num,char *str,int radix);

void IIC_WritePage_Adjust (void);
void IIC_ReadPage_Adjust(void);
void TIM6_Seconds_Counter(void);
#endif

