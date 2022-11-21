#include "stm32f10x.h"
#include "beep.h"
#include "led.h"

//****************************************************************************************
/*
��������:BEEP_config
��������:�������ĳ�ʼ��
������������
��������ֵ����
*/
void BEEP_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//����һ���ṹ���������ʼ��
	//��B�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//��PB8��ʼ��Ϊͨ������ģʽ50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//�ܽų�ʼ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ��ܽ�ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ܽ��ٶ�
	//�Զ˿ڳ�ʼ��
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}








