#ifndef _USART_H_
#define _USART_H_




//初始化串口USART1
void USART1_Config (void);



//通过串口USART1接受并发送数据
void USART_Echo (void);


void USART_js (void);



//通过串口控制LED灯的亮灭
void USART_Fun2 (void);





//重映射串口及初始化
void AFIO_CYS (void);

//回显
void AFIO_HX (void);

//用字符串控制灯亮、灯灭
void USART_ZFC (void);


void AFIO_ZFSZ (void);




//利用上层协议实现，控制四个灯一个蜂鸣器
void USART_SC (void);





//***************************************************


//初始化串口中断（USART）
void USART1_Zhong (void);


//串口中断控制LED函数
void USART1_TEXT (void);




#endif



