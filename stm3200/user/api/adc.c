#include "includes.h"

volatile u8 dma_buffer[] = {"����Ҽ"};
/*************************************************************************************
��������:ADC1_Config
��������:ADC1�ĳ�ʼ��
������������
��������ֵ����
***************************************************************************************/
void ADC1_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
		//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	 //��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//��ADC��ʱ�ӽ��з�Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
		//��ʼ��ADC1
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;   //����ģʽ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;    //��ͨ�����ǵ�ͨ��
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  //����ѭ��ģʽ��������ѭ��ģʽ
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //���¼����������������
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //ѡ���Ҷ���
	ADC_InitStruct.ADC_NbrOfChannel = 1;  //˳��ת���Ĺ�����ͨ����Ŀ
	ADC_Init(ADC1,&ADC_InitStruct);
	
		//ʹ��ADC1
	ADC_Cmd(ADC1,ENABLE);
	
		//��У׼ --- ʹ�ܻ�ʹ��У׼�Ĵ���  --- �ȴ���� --- ����У׼ --- �ȴ�У׼���
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/*************************************************************************************
��������:ADC1_More_Config
��������:ADC1�ĳ�ʼ��
������������
��������ֵ����
***************************************************************************************/
void ADC1_More_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
		//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
	
		//�Ծ���ADC��Ƶ�ʣ�APB2�����з�Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
		//��ʼ��GPIO��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;   //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStruct);

		//��ʼ��ADC
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;   //����ģʽ
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;   //��ͨ�����ǵ�ͨ��
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;   //����ѭ��ģʽ��������ѭ��ģʽ
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //���¼����������������
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //ѡ���Ҷ���
	ADC_InitStruct.ADC_NbrOfChannel = 2;  //˳��ת���Ĺ�����ͨ����Ŀ
	ADC_Init(ADC1,&ADC_InitStruct);
	
		//�����¶ȴ���������
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
		//ʹ�ø�λУ׼���ǳ�ʼ��У׼�Ĵ���
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus (ADC1) == SET);
	
		//��ʼADCУ׼
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	 //����ָ����ADC������ͨ�������С�����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,2,ADC_SampleTime_239Cycles5);
	
	Delay_ms(50);
}

/*************************************************************************************
��������:DMA_Config
��������:DMA�ĳ�ʼ��
������������
��������ֵ����
***************************************************************************************/
void DMA_Config (void)
{
  DMA_InitTypeDef DMA_InitStruct;
	
	  //��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	  //��ʼ��DMA
	DMA_InitStruct.DMA_BufferSize = 2;   //�������������贫����������---����ͨ����
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;  //���䷽��ADC��ΪԴ��
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  //�ڴ浽�ڴ治��Ҫ���ر�
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)dma_buffer;  //�ڴ����ַ
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //�ڴ����ݿ��
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;  //ѭ��ģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //�������ַ
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�������ݿ��
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ������
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;  //����ͨ�����ȼ�
	DMA_Init (DMA1_Channel1,&DMA_InitStruct);  //��ʼ��DMA
	
	ADC_DMACmd(ADC1,ENABLE);  //��ADC1  DMAͨ��
//	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  //ʹ�ܴ��ڣ�ʹDMA�����ڴ�������
//	
	DMA_Cmd(DMA1_Channel1,ENABLE);  //ʹ��DMA
}

/*************************************************************************************
��������:Get_adcvalue
��������:��ȡADC��ֵ
����������u8 channel ��ADC��ѡ���ͨ��
					 u16 *value ��ָ��ADC����ֵ��ָ��
��������ֵ����
***************************************************************************************/
void Get_adcvalue(u8 channel,u16 *value)
{
		//����ָ���Ĺ���ͨ��������ת��˳�򣬲���ʱ��
		ADC_RegularChannelConfig(ADC1,channel,1,ADC_SampleTime_239Cycles5);
	
		//�������ADCת��
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
		//�ȴ�ADCת�����
		while (ADC_GetSoftwareStartConvStatus(ADC1) == SET);
	
		//��ȡ���һ��ADCת���Ľ��
		*value = ADC_GetConversionValue(ADC1);
}

/*************************************************************************************
��������:Adc_test
��������:���Զ�ȡ�ⲿ�����������ĵ�ѹֵ
������������
��������ֵ����
***************************************************************************************/
void Adc_test (void)
{
	u16 temp = 0;
	float ad_value;
	 // ��ȡADCת����ֵ
	Get_adcvalue(1,&temp);
	printf("%#x\r\n",temp);
	ad_value = (3.3*(float)temp)/4096.0;
	printf("%f\r\n",ad_value);
	Delay_ms(500);
}

/*************************************************************************************
��������:Get_More_adcvalue
��������:��ȡADC��ֵ
����������u8 lenth :����Ĵ�С
					 u16 *value ���������ݵ�����
��������ֵ����
***************************************************************************************/
void Get_More_adcvalue (u8 lenth,u16 *value)
{
		int i = 0;
		u32 time_out = 0;
			//�������
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	for(i=0;i<lenth;i++)
	{
		 //�ȴ�ADCת�����
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
		
		ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
		value[i] = ADC_GetConversionValue(ADC1);
	}
		//����ر�ADCת��
	ADC_SoftwareStartConvCmd(ADC1,DISABLE);
}

/*************************************************************************************
��������:Adc_More_test
��������:���Զ�ȡ�ⲿ�����������ĵ�ѹֵ
������������
��������ֵ����
***************************************************************************************/
void Adc_More_test(void)
{
	u16 temp = 0;
	float ad_value;
	Get_adcvalue(2,&temp);
	printf("%#x\r\n",temp);
	ad_value = (3.3*(float)temp)/4096.0;
	printf("%f\r\n",ad_value);
	Delay_ms(500);
}

/*************************************************************************************
��������:Adc_morechanneltest
��������:����оƬ�¶ȵ�ֵ
������������
��������ֵ����
***************************************************************************************/
void Adc_morechanneltest(void)
{
	u16 temp[2];
	u16 i = 0;
	float ad_value;
	float temp_value;
	Get_adcvalue(2,temp);
	for(i=0;i<2;i++)
		printf("temp[%d] =%d\r\n",i,temp[i]);
	 //ADC1
	ad_value = (3.3*(float)temp[0])/4096.0;
	printf("%f\r\n",ad_value);
	    //�¶�
	temp_value = (3.3*(float)temp[1])/4096.0;
	//printf("%f\r\n",temp_value);
	temp_value=(1.43-temp_value)/0.0043+25;
	printf("%f\r\n",temp_value);
	Delay_ms(500);
}
