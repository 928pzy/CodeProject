#ifndef _LED_H_
#define _LED_H_




//����Ŀ�������(��ˮ��)��������
#define LED1(x)  x?(GPIOB->ODR &= ~(1<<5)):(GPIOB->ODR |= (1<<5))
#define LED2(x)  x?(GPIOE->ODR &= ~(1<<5)):(GPIOE->ODR |= (1<<5))
#define LED3(x)  x?(GPIOA->ODR &= ~(1<<5)):(GPIOA->ODR |= (1<<5))
#define LED4(x)  x?(GPIOA->ODR &= ~(1<<6)):(GPIOA->ODR |= (1<<6))





//ģʽ����������
void Led_cnf (void);





//����LED���Ӻ���������
void LED1_Config(void);
void LED2_Config(void);
void LED3_Config(void);
void LED4_Config(void);





//�����Ƶ�ʵ��
void LED_hx (void);






//��ˮ�Ƶ�ʵ��
void LED_ss (void);







#endif



