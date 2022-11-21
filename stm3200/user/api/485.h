#ifndef _485_H_
#define _485_H_



#define USA_485(x) x?(GPIO_SetBits(GPIOG,GPIO_Pin_9)):(GPIO_ResetBits(GPIOG,GPIO_Pin_9))
#define   MAXSIZE    64
typedef struct 
{
	u8 elem[MAXSIZE];
	u8 *front;
	u8 *end;
	u8 *indata;
	u8 *outdata;
	u8 lenth;
}QUEUElen;



void USART_485 (void);
void USART2_Config (void);
void USART_Get_485 (void);
void USART_Send_485 (void);
void RS485QUEUE_Receive (int ch);
u8 RS485QUEUE_Transmit (void);
void RS485Config(void);






#endif 


