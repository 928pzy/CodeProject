#include "includes.h"



/****************************************************************************************
������:DHT11_InInit
�βΣ���
����ֵ����
�������ܣ���ʼ�������ģʽ
**************************************************************************/
void DHT11_InInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//��GPIO�ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	 //��ʼ��GPIO�ڵ�����ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  //DHT11�д����������裬��Ҫ����
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	DTH11_DATA_IO_H();  //����Ҫ����Ĺܽ�����
}

/****************************************************************************************
������:DHT11_OutInit
�βΣ���
����ֵ����
�������ܣ���ʼ�������ģʽ
**************************************************************************/
void DHT11_OutInit (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	DTH11_DATA_IO_H();   //����Ҫ����Ĺܽ�����
}

/**************************************************
������:RH
�βΣ���
����ֵ����
�������ܣ���ȡ��ʪ������
***************************************************/
u8 RH(u8 *Pdata)
{
    u8 flag=0;
    
	  /*�������Ϳ�ʼ�ź�*/
	  DHT11_OutInit();     //�������ģʽ��ͨ�ÿ�©�����
	
    DTH11_DATA_IO_L();   //���͵�ƽ
    Delay_ms(18);        //��ʱ18����
    DTH11_DATA_IO_H();   //���ߵ�ƽ
	
   	/*�ӻ���Ӧ*/
   	DHT11_InInit();      //��������ģʽ��ͨ���������룩
    while(RD_DTH11_DATA_IO()); //�ȴ��ӻ���Ӧ�������������͵�ƽ
 
    if(!RD_DTH11_DATA_IO()) 	  //ȷ���Ƿ������Ӧ��
    {
        while(!RD_DTH11_DATA_IO());   //�ȴ���ƽ��ƽ����
        
        while(RD_DTH11_DATA_IO());    //���͵�ƽ��֪ͨ����׼����������
        
					/*��ʼ��������*/
        *Pdata = ReadData();
        *(Pdata + 1) = ReadData();
        *(Pdata + 2) = ReadData();
        *(Pdata + 3) = ReadData();
        *(Pdata + 4) = ReadData();
		
			 DHT11_OutInit();  //�������ģʽ
       DTH11_DATA_IO_H();//�ͷ�����
    }
    flag = CheckSum(Pdata);
    return flag;
}  
	
/**************************************************
������:ReadData
�βΣ���
����ֵ����
�������ܣ���DHT11�ж�ȡ����
***************************************************/
u8 ReadData(void)	
{
    u8 i; 
    u8 temp=0;
    u8 comdata=0;
   
    for(i=0;i<8;i++)	
    {	
        while(!RD_DTH11_DATA_IO());  //�ȴ���������
				Delay_us(30);                //��ʱ
									 //                      �ж���ʱʱ����ȷ�����ܵ�������Ϊ0��Ϊ1
        if(RD_DTH11_DATA_IO())       
        {
            temp=1;
        }
        else 
        {
            temp = 0;
        }
        while(RD_DTH11_DATA_IO());  //�ȴ��´η������ݲ����͵�ƽ
        
        comdata<<=1;
        comdata|=temp;        
    }
    return comdata;
}

/**************************************************
������:CheckSum
�βΣ���У��������׵�ַ
����ֵ��У���Ƿ�ɹ�
�������ܣ�У����ܵ�������
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
������:DHT11_Test
�βΣ���
����ֵ����
�������ܣ����Ժ�����ͨ�����ڣ�
***************************************************/
void DHT11_Test(void)
{
	u8 DHT11_Data[5];
	u8 buffer[32];
	
	Delay_ms(1000);
	if(RH(DHT11_Data) == 1)    //�жϽ��ܵ��������Ƿ���ȷ
	{
		printf("ʪ�ȣ�");
		buffer[0] = (DHT11_Data[0]/10)%10 + '0';
		buffer[1] = (DHT11_Data[0]%10) + '0';
		buffer[2] = '\0';
		
		printf("%s\r\n",buffer);
		
		printf("�¶ȣ�");
		buffer[0] = (DHT11_Data[2]/10)%10 + '0';
		buffer[1] = (DHT11_Data[2]%10) + '0';
		buffer[2] = '\0';
		printf("%s\r\n",buffer);
	}
}

/**************************************************
������:Show_DHT11Lcd
�βΣ���
����ֵ����
�������ܣ����Ժ�����ͨ��LCD����
***************************************************/
void Show_DHT11Lcd(void)
{
	u8 DHT11_Data[5];
	char buffer_shidu[10] = {"shidu=   "};
	char buffer_wendu[10] = {"wendu=   "};
	
	Delay_ms(1000);
	
	if(RH(DHT11_Data) == 1)   //�����ȡ���������Ƿ���ȷ  
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














