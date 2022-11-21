#include "includes.h"

/*************************************************************************************
函数名称:
函数功能:
函数参数：无
函数返回值：无
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
	
	USART1_Config();  //串口初始化
	
	Systick_Config();  //滴答时钟初始化
	
	KEY_config();    //按键初始化
	
//	FLASH_Init();
//	
//	IIC_Init();    //初始化写入芯片的协议
//	
//	LCD_Init();   //屏幕初始化
//	
//	TIM6_Config(360,1000);  //初始化基准时钟
	
//	NVIC_Config();			//初始化中断通道
//	
//	TP_GPIOConfig();   //触摸屏初始化
//	
//	IIC_ReadPage_Adjust();  //把系数从芯片中读取出来
//	
//	GUI_RectangleFill(80,240,110,280,BLACK);
//	GUI_RectangleFill(130,240,160,280,BLACK);

//		TIM4_Config (72,200);
//		TIM3_Config();
//   ADC1_More_Config();
//	 DMA_Config();
	 
//	 ADC_SoftwareStartConvCmd(ADC1,ENABLE);   //使能ADC软件转换启动功能，DMA保存采集到的数据
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
				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);  //判断是否发送成功
				USART_SendData(USART2,send_num);
				printf("发送的数据为:%d\n\r",send_num);
		}		
	}
}
	

	
	
	
	




