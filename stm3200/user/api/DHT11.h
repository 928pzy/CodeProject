#ifndef _DHT11_H_
#define _DHT11_H_

extern u8 Temperature_ten;   //温度的十位
extern u8 Temperature_unit;  //温度的个位
extern u8 Humidity_ten;      //湿度的十位
extern u8 Humidity_unit;     //湿度的个位


#define DTH11_DATA_IO_H()   GPIO_SetBits(GPIOG,GPIO_Pin_11)
#define DTH11_DATA_IO_L()   GPIO_ResetBits(GPIOG,GPIO_Pin_11)

#define RD_DTH11_DATA_IO()  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11)

void DHT11_InInit(void);
void DHT11_OutInit(void);

u8 CheckSum(u8 *Pdata);
u8 ReadData(void);
u8 RH(u8 *Pdata);
void DHT11_Test(void);
void Show_DHT11Lcd(void);

#endif

