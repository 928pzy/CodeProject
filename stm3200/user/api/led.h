#ifndef _LED_H_
#define _LED_H_




//用三目运算符来(流水灯)声明函数
#define LED1(x)  x?(GPIOB->ODR &= ~(1<<5)):(GPIOB->ODR |= (1<<5))
#define LED2(x)  x?(GPIOE->ODR &= ~(1<<5)):(GPIOE->ODR |= (1<<5))
#define LED3(x)  x?(GPIOA->ODR &= ~(1<<5)):(GPIOA->ODR |= (1<<5))
#define LED4(x)  x?(GPIOA->ODR &= ~(1<<6)):(GPIOA->ODR |= (1<<6))





//模式的声明函数
void Led_cnf (void);





//各个LED灯子函数的声明
void LED1_Config(void);
void LED2_Config(void);
void LED3_Config(void);
void LED4_Config(void);





//呼吸灯的实现
void LED_hx (void);






//流水灯的实现
void LED_ss (void);







#endif



