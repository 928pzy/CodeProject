#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"


#define KEY_UP()    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取管脚的输入值
#define KEY_RIGHT() GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY_DOWN()  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY_LEFT()  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)


//按键初始化
void KEY_config (void);




//按键功能返回值
u8 Scan_KeyValue (void);//使用两个键的函数
void Scan2_KeyValue (void);//使用四个键的函数

//一个按键控制灯的亮灭
void Key (void);


//状态机实现按键控制流水灯
void Get_keyup (void);

void Get_keydown (void);

void Get_keyright (void);

void Get_keylift (void);









#endif







