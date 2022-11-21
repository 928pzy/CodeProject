#include "includes.h"

volatile u8 dma_buffer[] = {"潘振壹"};
/*************************************************************************************
函数名称:ADC1_Config
函数功能:ADC1的初始化
函数参数：无
函数返回值：无
***************************************************************************************/
void ADC1_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
		//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	 //初始化GPIO口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//对ADC的时钟进行分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
		//初始化ADC1
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;   //独立模式
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;    //多通道还是单通道
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  //单次循环模式还是连续循环模式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //有事件触发还是软件触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //选择右对齐
	ADC_InitStruct.ADC_NbrOfChannel = 1;  //顺序转换的规则组通道数目
	ADC_Init(ADC1,&ADC_InitStruct);
	
		//使能ADC1
	ADC_Cmd(ADC1,ENABLE);
	
		//自校准 --- 使能或使能校准寄存器  --- 等待完成 --- 启动校准 --- 等待校准完成
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/*************************************************************************************
函数名称:ADC1_More_Config
函数功能:ADC1的初始化
函数参数：无
函数返回值：无
***************************************************************************************/
void ADC1_More_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
		//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
	
		//对经过ADC的频率（APB2）进行分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
		//初始化GPIO口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;   //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStruct);

		//初始化ADC
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;   //独立模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;   //多通道还是单通道
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;   //单次循环模式还是连续循环模式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //有事件触发还是软件触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //选择右对齐
	ADC_InitStruct.ADC_NbrOfChannel = 2;  //顺序转换的规则组通道数目
	ADC_Init(ADC1,&ADC_InitStruct);
	
		//开启温度传感器功能
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
		//使用复位校准就是初始化校准寄存器
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus (ADC1) == SET);
	
		//开始ADC校准
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	 //设置指定的ADC规则组通道：序列、采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,2,ADC_SampleTime_239Cycles5);
	
	Delay_ms(50);
}

/*************************************************************************************
函数名称:DMA_Config
函数功能:DMA的初始化
函数参数：无
函数返回值：无
***************************************************************************************/
void DMA_Config (void)
{
  DMA_InitTypeDef DMA_InitStruct;
	
	  //打开时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	  //初始化DMA
	DMA_InitStruct.DMA_BufferSize = 2;   //传输数据量（需传输两个数据---两个通道）
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;  //传输方向（ADC作为源）
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  //内存到内存不需要，关闭
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)dma_buffer;  //内存基地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //内存数据宽度
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址增加
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;  //循环模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //外设基地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设数据宽度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不增加
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;  //设置通道优先级
	DMA_Init (DMA1_Channel1,&DMA_InitStruct);  //初始化DMA
	
	ADC_DMACmd(ADC1,ENABLE);  //打开ADC1  DMA通道
//	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  //使能串口，使DMA给串口传输数据
//	
	DMA_Cmd(DMA1_Channel1,ENABLE);  //使能DMA
}

/*************************************************************************************
函数名称:Get_adcvalue
函数功能:获取ADC的值
函数参数：u8 channel ：ADC所选择的通道
					 u16 *value ：指向ADC返回值的指针
函数返回值：无
***************************************************************************************/
void Get_adcvalue(u8 channel,u16 *value)
{
		//设置指定的规则通道，设置转化顺序，采样时间
		ADC_RegularChannelConfig(ADC1,channel,1,ADC_SampleTime_239Cycles5);
	
		//软件启动ADC转换
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
		//等待ADC转换完成
		while (ADC_GetSoftwareStartConvStatus(ADC1) == SET);
	
		//获取最近一次ADC转换的结果
		*value = ADC_GetConversionValue(ADC1);
}

/*************************************************************************************
函数名称:Adc_test
函数功能:测试读取外部滑动变阻器的电压值
函数参数：无
函数返回值：无
***************************************************************************************/
void Adc_test (void)
{
	u16 temp = 0;
	float ad_value;
	 // 获取ADC转换的值
	Get_adcvalue(1,&temp);
	printf("%#x\r\n",temp);
	ad_value = (3.3*(float)temp)/4096.0;
	printf("%f\r\n",ad_value);
	Delay_ms(500);
}

/*************************************************************************************
函数名称:Get_More_adcvalue
函数功能:获取ADC的值
函数参数：u8 lenth :数组的大小
					 u16 *value ：接受数据的数组
函数返回值：无
***************************************************************************************/
void Get_More_adcvalue (u8 lenth,u16 *value)
{
		int i = 0;
		u32 time_out = 0;
			//软件启动
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	for(i=0;i<lenth;i++)
	{
		 //等待ADC转换完成
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
		
		ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
		value[i] = ADC_GetConversionValue(ADC1);
	}
		//软件关闭ADC转换
	ADC_SoftwareStartConvCmd(ADC1,DISABLE);
}

/*************************************************************************************
函数名称:Adc_More_test
函数功能:测试读取外部滑动变阻器的电压值
函数参数：无
函数返回值：无
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
函数名称:Adc_morechanneltest
函数功能:测试芯片温度的值
函数参数：无
函数返回值：无
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
	    //温度
	temp_value = (3.3*(float)temp[1])/4096.0;
	//printf("%f\r\n",temp_value);
	temp_value=(1.43-temp_value)/0.0043+25;
	printf("%f\r\n",temp_value);
	Delay_ms(500);
}
