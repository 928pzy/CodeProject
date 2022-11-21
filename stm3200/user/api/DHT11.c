#include "includes.h"



/****************************************************************************************
函数名:DHT11_InInit
形参：无
返回值：无
函数功能：初始化输入的模式
**************************************************************************/
void DHT11_InInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//打开GPIO口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	 //初始化GPIO口的输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  //DHT11中存在上拉电阻，需要上拉
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	DTH11_DATA_IO_H();  //把需要输入的管脚拉高
}

/****************************************************************************************
函数名:DHT11_OutInit
形参：无
返回值：无
函数功能：初始化输出的模式
**************************************************************************/
void DHT11_OutInit (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	DTH11_DATA_IO_H();   //把需要输出的管脚拉高
}

/**************************************************
函数名:RH
形参：有
返回值：有
函数功能：读取温湿度数据
***************************************************/
u8 RH(u8 *Pdata)
{
    u8 flag=0;
    
	  /*主机发送开始信号*/
	  DHT11_OutInit();     //配置输出模式（通用开漏输出）
	
    DTH11_DATA_IO_L();   //拉低电平
    Delay_ms(18);        //延时18毫秒
    DTH11_DATA_IO_H();   //拉高电平
	
   	/*从机回应*/
   	DHT11_InInit();      //配置输入模式（通用上拉输入）
    while(RD_DTH11_DATA_IO()); //等待从机回应，数据总线拉低电平
 
    if(!RD_DTH11_DATA_IO()) 	  //确认是否真的有应答
    {
        while(!RD_DTH11_DATA_IO());   //等待电平电平拉高
        
        while(RD_DTH11_DATA_IO());    //拉低电平，通知主机准备接受数据
        
					/*开始接受数据*/
        *Pdata = ReadData();
        *(Pdata + 1) = ReadData();
        *(Pdata + 2) = ReadData();
        *(Pdata + 3) = ReadData();
        *(Pdata + 4) = ReadData();
		
			 DHT11_OutInit();  //配置输出模式
       DTH11_DATA_IO_H();//释放总线
    }
    flag = CheckSum(Pdata);
    return flag;
}  
	
/**************************************************
函数名:ReadData
形参：无
返回值：无
函数功能：从DHT11中读取数据
***************************************************/
u8 ReadData(void)	
{
    u8 i; 
    u8 temp=0;
    u8 comdata=0;
   
    for(i=0;i<8;i++)	
    {	
        while(!RD_DTH11_DATA_IO());  //等待数据拉高
				Delay_us(30);                //延时
									 //                      判断延时时长，确定接受到的数据为0或为1
        if(RD_DTH11_DATA_IO())       
        {
            temp=1;
        }
        else 
        {
            temp = 0;
        }
        while(RD_DTH11_DATA_IO());  //等待下次发送数据并拉低电平
        
        comdata<<=1;
        comdata|=temp;        
    }
    return comdata;
}

/**************************************************
函数名:CheckSum
形参：需校验的数据首地址
返回值：校验是否成功
函数功能：校验接受到的数据
***************************************************/
u8 CheckSum(u8 *Pdata)
{
	if (*(Pdata+4) == (*(Pdata+3)+ *(Pdata+2)+ *(Pdata+1)+ *Pdata))
	{
			return 1;
	}
			return 0;
}

/**************************************************
函数名:DHT11_Test
形参：无
返回值：无
函数功能：测试函数（通过串口）
***************************************************/
void DHT11_Test(void)
{
	u8 DHT11_Data[5];
	u8 buffer[32];
	
	Delay_ms(1000);
	if(RH(DHT11_Data) == 1)    //判断接受到的数据是否正确
	{
		printf("湿度：");
		buffer[0] = (DHT11_Data[0]/10)%10 + '0';
		buffer[1] = (DHT11_Data[0]%10) + '0';
		buffer[2] = '\0';
		
		printf("%s\r\n",buffer);
		
		printf("温度：");
		buffer[0] = (DHT11_Data[2]/10)%10 + '0';
		buffer[1] = (DHT11_Data[2]%10) + '0';
		buffer[2] = '\0';
		printf("%s\r\n",buffer);
	}
}

/**************************************************
函数名:Show_DHT11Lcd
形参：无
返回值：无
函数功能：测试函数（通过LCD屏）
***************************************************/
void Show_DHT11Lcd(void)
{
	u8 DHT11_Data[5];
	char buffer_shidu[10] = {"shidu=   "};
	char buffer_wendu[10] = {"wendu=   "};
	
	Delay_ms(1000);
	
	if(RH(DHT11_Data) == 1)   //检验读取到的数据是否正确  
	{
		buffer_shidu[6] = (DHT11_Data[0]/10)%10 + '0';
		buffer_shidu[7] = (DHT11_Data[0]%10) + '0';
		buffer_shidu[8] = '\0';
		
		buffer_wendu[6] = (DHT11_Data[2]/10)%10 + '0';
		buffer_wendu[7] = (DHT11_Data[2]%10) + '0';
		buffer_wendu[8] = '\0';
	}
	
	Draw_Text_8_16_Str(20,120,RED,WHITE,(u8 *)buffer_shidu);
	Draw_Text_8_16_Str(20,160,RED,WHITE,(u8 *)buffer_wendu);
}














