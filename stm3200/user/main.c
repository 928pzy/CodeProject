#include "includes.h"

/*************************************************************************************
��������:
��������:
������������
��������ֵ����
***************************************************************************************/
//extern float IC2Value;
//extern float DutyCycle;
//extern float Frequency;
//extern float IC1Value;
//extern u32 tim3_num;

extern u8 dma_buffer[];

int main ()
{	
	u8 send_num;
	
	Led_cnf();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	USART1_Config();  //���ڳ�ʼ��
	
	Systick_Config();  //�δ�ʱ�ӳ�ʼ��
	
	KEY_config();    //������ʼ��
	
//	FLASH_Init();
//	
//	IIC_Init();    //��ʼ��д��оƬ��Э��
//	
//	LCD_Init();   //��Ļ��ʼ��
//	
//	TIM6_Config(360,1000);  //��ʼ����׼ʱ��
	
//	NVIC_Config();			//��ʼ���ж�ͨ��
//	
//	TP_GPIOConfig();   //��������ʼ��
//	
//	IIC_ReadPage_Adjust();  //��ϵ����оƬ�ж�ȡ����
//	
//	GUI_RectangleFill(80,240,110,280,BLACK);
//	GUI_RectangleFill(130,240,160,280,BLACK);

//		TIM4_Config (72,200);
//		TIM3_Config();
//   ADC1_More_Config();
//	 DMA_Config();
	 
//	 ADC_SoftwareStartConvCmd(ADC1,ENABLE);   //ʹ��ADC���ת���������ܣ�DMA����ɼ���������
	RS485Config();
	while(1)
	{
//		TIM6_Seconds_2();
//		TIM6_Seconds_Counter();
//			printf("first  = %d\r\n",dma_buffer[0]);
//		  printf("second = %d\r\n",dma_buffer[1]);
//		  Delay_ms(2000);
//		USART_Get_485();
//			USART_Send_485();
		if (Scan_KeyValue() == 1)
		{
				USA_485(1);
				send_num = (u8)rand();
				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);  //�ж��Ƿ��ͳɹ�
				USART_SendData(USART2,send_num);
				printf("���͵�����Ϊ:%d\n\r",send_num);
		}		
	}
}
	

	
	
	
	




