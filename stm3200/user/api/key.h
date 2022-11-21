#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"


#define KEY_UP()    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ�ܽŵ�����ֵ
#define KEY_RIGHT() GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY_DOWN()  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY_LEFT()  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)


//������ʼ��
void KEY_config (void);




//�������ܷ���ֵ
u8 Scan_KeyValue (void);//ʹ���������ĺ���
void Scan2_KeyValue (void);//ʹ���ĸ����ĺ���

//һ���������ƵƵ�����
void Key (void);


//״̬��ʵ�ְ���������ˮ��
void Get_keyup (void);

void Get_keydown (void);

void Get_keyright (void);

void Get_keylift (void);









#endif







