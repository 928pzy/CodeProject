#ifndef _ADC_H_
#define _ADC_H_



void ADC1_Config (void);
void Get_adcvalue(u8 channel,u16 *value);
void Adc_test (void);

void ADC1_More_Config (void);
void Get_More_adcvalue (u8 lenth,u16 *value);
void Adc_More_test(void);
void Adc_morechanneltest(void);

void DMA_Config (void);










#endif 


