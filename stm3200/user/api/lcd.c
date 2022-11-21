#include "includes.h"

/****************************************************
��������Delay
�βΣ� ��
����ֵ����
�������ܣ���ʱ����
****************************************************/
static void Delay(u32 time)
{    
   u16 i=0;  
   while(time--)
   {
	  i=12000;  //�Լ�����
	  while(i--) ;    
   }
}

//����LCD��Ҫ������Ĭ��Ϊ����
_lcd_dev lcddev;
/****************************************************
��������LCD_WR_REG
�βΣ�  regval:�Ĵ���ֵ
����ֵ����
�������ܣ�д�Ĵ�������
****************************************************/
void LCD_WR_REG(u16 regval)
{ 
	LCD->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
/****************************************************
��������LCD_WR_DATA
�βΣ� data:Ҫд���ֵ
����ֵ����
�������ܣ�дLCD����(д�������Ϊ16λ����ɫֵ)
****************************************************/
void LCD_WR_DATA(u16 data)
{										    	   
	LCD->LCD_RAM=data;		 
}
/****************************************************
��������LCD_RD_DATA
�βΣ���
����ֵ����ȡ��16λ��ɫֵ
�������ܣ���LCD����
****************************************************/
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   
/****************************************************
��������LCD_WriteReg
�βΣ�LCD_Reg:�Ĵ�����ַ   LCD_RegValue:Ҫд�������
����ֵ����
�������ܣ�д�Ĵ���
****************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������		 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
/****************************************************
��������LCD_ReadReg
�βΣ�LCD_Reg:�Ĵ�����ַ
����ֵ������������
�������ܣ����Ĵ���
****************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	Delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   
/****************************************************
��������LCD_WriteRAM_Prepare
�βΣ���
����ֵ����
�������ܣ���ʼдGRAM
****************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  //���Ϳ�ʼд���ָ��
}	
/****************************************************
��������LCD_WriteRAM
�βΣ�RGB_Code:��ɫֵ
����ֵ����
�������ܣ�LCDдGRAM
****************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A23 ��LCD��RS(�Ĵ������/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6D00 0000 = 0X6C00 0000+2^23*2 = 0X6C00 0000 + 0X100 0000 = 0X6D00 0000
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/
/****************************************************
��������LCD_GPIO_Config
�βΣ���
����ֵ����
�������ܣ���ʼ��LCD���ӿڵ�GPIO��
****************************************************/
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ��FSMCʱ��*/
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOD \
	 					  |RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOG,\
	                       ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* ����LCD������ƹܽ�*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ����LCD��λ���ƹܽ�*/
    //�õ���RESET�ܽ�


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	/* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1 8 9 10,PE 7 8 9 10 11 12 13 14 15*/	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1\
	                              |GPIO_Pin_8| GPIO_Pin_9\
	                              |GPIO_Pin_10|GPIO_Pin_14\
	                              |GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8\
	 							| GPIO_Pin_9 | GPIO_Pin_10\
	 						    | GPIO_Pin_11 | GPIO_Pin_12\
	 						    | GPIO_Pin_13 | GPIO_Pin_14\
	   						    | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* ����FSMC���Ӧ�Ŀ�����
	* PD4-FSMC_NOE   :LCD-RD
	* PD5-FSMC_NWE   :LCD-WR
	* PG12-FSMC_NE4  :LCD-CS
	* PE2-FSMC_A23   :LCD-DC
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//PD4-FSMC_NOE   :LCD-RD

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//PD5-FSMC_NWE   :LCD-WR
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //PG12-FSMC_NE4  :LCD-CS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //PE2-FSMC_A10   :LCD-DC���͵����ݴ������������ʾ����

 		    
}
/****************************************************
��������LCD_FSMC_Config
�βΣ���
����ֵ����
�������ܣ�FSMC�ĳ�ʼ��
****************************************************/
void LCD_FSMC_Config(void)
{
	
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  WRITE_USER; 
	FSMC_NORSRAMTimingInitTypeDef  READ_USER; 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	READ_USER.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ��
    READ_USER.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    READ_USER.FSMC_DataSetupTime = 0x0f;		 //���ݽ���ʱ��
    READ_USER.FSMC_BusTurnAroundDuration = 0x00;
    READ_USER.FSMC_CLKDivision = 0x00;
    READ_USER.FSMC_DataLatency = 0x00;
    READ_USER.FSMC_AccessMode = FSMC_AccessMode_A;	 // 

	WRITE_USER.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ��
    WRITE_USER.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    WRITE_USER.FSMC_DataSetupTime = 0x03;		 //���ݽ���ʱ��
    WRITE_USER.FSMC_BusTurnAroundDuration = 0x00;//
    WRITE_USER.FSMC_CLKDivision = 0x00;//
    WRITE_USER.FSMC_DataLatency = 0x00;//
    WRITE_USER.FSMC_AccessMode = FSMC_AccessMode_A;	 // 
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;  //ѡ����Ӵ洢����ĵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//���������ߺ͵�ַ���Ƿ���
//    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;//������Ӵ洢��������
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//����FSMC�ӿڵ����ݿ��
	
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//���÷���ģʽ
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;//���õȴ��źŵļ���
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;//�����Ƿ�ʹ�÷Ƕ��뷽ʽ
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//���õȴ��ź�ʲôʱ�����
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;//�����Ƿ�ʹ�õȴ��ź�
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;//�����Ƿ�����ͻ��д����
	
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//����д����ʹ��
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable ;//�����Ƿ�ʹ����չģʽ
    
	
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &READ_USER;//��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &WRITE_USER;  //дʱ��
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* ʹ�� FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);

}
/****************************************************
��������LCD_init_RGB
�βΣ���
����ֵ����
�������ܣ�LCD��ILI9341�������ĳ�ʼ��
****************************************************/
void LCD_init_RGB(void)
{
	Delay(50); // delay 50 ms 
	
 	LCD_WriteReg(0x0000,0x0001);
	 
	Delay(50); // delay 50 ms 
  	lcddev.id = LCD_ReadReg(0x0000);  
	 
  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//����ID����ȷ,����lcddev.id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300
	{	
 		//����9341 ID�Ķ�ȡ		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 				//dummy read 	
 		LCD_RD_DATA();   	    	//����0X00
  		lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	   			   
 		
		if(lcddev.id!=0X9341)lcddev.id=0x9341;//����ʶ��9341 	     
	}
	 
	if(lcddev.id==0X9341)	//9341��ʼ��  ʹ�õ�����չģʽ
	{	 
		LCD_WR_REG(0xCF);      
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81);
		
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02);
		
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20);
		
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);
		
		LCD_WR_REG(0x11); //Exit Sleep
		Delay(120);
		LCD_WR_REG(0x29); //display on	
	}
}
/****************************************************
��������LCD_Init
�βΣ���
����ֵ����
�������ܣ�LCD���ĳ�ʼ��
****************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config();
	
	LCD_FSMC_Config();
	
	LCD_init_RGB();
		
	/* ������ */
	LCD_DisplayOn();
	
	LCD_Display_Dir(0);		 	//Ĭ��Ϊ����
	
	LCD_Clear(WHITE);	
}


//Xpos:������

//Ypos:������
/****************************************************
��������LCD_SetCursor
�βΣ�u16 Xpos -- X����, u16 Ypos -- Y����
����ֵ����
�������ܣ�����LCD������ֵ
****************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{    
      if(lcddev.id==0X9341)
      {               
		  LCD_WR_REG(lcddev.setxcmd);
		  LCD_WR_DATA(Xpos>>8);
		  LCD_WR_DATA(Xpos&0XFF);  
		  LCD_WR_REG(lcddev.setycmd);
		  LCD_WR_DATA(Ypos>>8);
		  LCD_WR_DATA(Ypos&0XFF);
      }
} 
/****************************************************
��������LCD_Scan_Dir
�βΣ�dir:0~7,����8������(���嶨���lcd.h) 
����ֵ����
�������ܣ�����LCD���Զ�ɨ�跽��һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.��
****************************************************/  
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//����ʱ����6804���ı�ɨ�跽��
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804)//9341/6804,������
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08;//BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);
		LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);
		LCD_WR_DATA((lcddev.width-1)&0XFF);
		
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
  	}
} 
/****************************************************
��������LCD_Display_Dir
�βΣ�dir:0,������1,����
����ֵ����
�������ܣ�����LCD��ʾ����
****************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}
	}
	else 				//����
	{	  				
		lcddev.dir=1;	//����
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}
/****************************************************
��������LCD_DrawPoint
�βΣ�x,y:����
����ֵ��POINT_COLOR:�˵����ɫ
�������ܣ����㺯��
****************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 c)
{	 
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM ��LCD������ʾԭ������������������
	LCD->LCD_RAM=c; 
}
/****************************************************
��������LCD_ReadPoint
�βΣ�x,y:����
����ֵ���˵����ɫ
�������ܣ���ȡ��ĳ�����ɫֵ
****************************************************/
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	 
	
	if(lcddev.id==0X9341)LCD_WR_REG(0X2E);//9341/6804/3510 ���Ͷ�GRAMָ��	    
	if(LCD->LCD_RAM)r=0;							//dummy Read	   
	Delay(2);	  
 	r=LCD->LCD_RAM;  		  						//ʵ��������ɫ
 	if(lcddev.id==0X9341)		//9341/NT35310/NT35510Ҫ��2�ζ���
 	{
		Delay(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;		//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	}
	return g;
}
/****************************************************
��������LCD_DisplayOn
�βΣ���
����ֵ����
�������ܣ���LCD����ʾ����
****************************************************/
void LCD_DisplayOn(void)//����ʾ
{
	/* ������ */
	GPIO_SetBits(GPIOB, GPIO_Pin_0);	
}
/****************************************************
��������LCD_DisplayOff
�βΣ���
����ֵ����
�������ܣ��ر�LCD����ʾ����
****************************************************/
void LCD_DisplayOff(void)//����ʾ
{
	/*�ر��� */
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

//��������
//color:Ҫ���������ɫ
/****************************************************
��������LCD_Clear
�βΣ�u16 color -- ��������ɫֵ
����ֵ����
�������ܣ�����
****************************************************/
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù�꣨LCD��������λ��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}
}  
/**************************************************************************
***** ��������  void  show_xXx(u32 x ,u32 y,u16 backColor,u16 color,const u8 *ch ,u16 size_line,u16 size_bits)
***** ���ܣ�    ���ƴ�СΪsize_line*size_bits�ĺ���
***** ������    x��������   y��������     color������ɫ  
   	            backColor  ���ֱ�����ɫ  ch����ģ����
								size_line:��     size_bits����
***** ����ֵ��	��
****************************************************************************/
void  show_xXx(u32 x ,u32 y,u16 backColor,u16 color,const u8 *ch ,u16 size_line,u16 size_bits)
{
	//ǰ����ɫ�ͱ�����ɫ�Ѿ��趨��
	int x0,y0;
	u32 dat;
	int i;
	for(y0=0;y0<size_line;y0++)                   //����
	{                              //0x10,0x02 0x1000 | 0x02 = 0x1002
		    //���ֿ��Ϊ16���������ֽ����һ��16λ����
		for(i=0;i<size_bits/8;i++)    //ÿ��ȡ�����ֽڣ������ֽڴ���һ�У�һ�����ݿ��Դ�8���㣬ÿλ1�����ǰ����ɫ��0����򱳾���ɫ
		{
			dat = *ch++;						
			for(x0=0;x0<8;x0++)             //����
			{
				//����
				if(dat&0x80)                  //����߿���㣬ֻ���ж�һλ
					LCD_DrawPoint(x+(x0+i*8),y+y0,color);    //���ַ���ɫ
				else
				{
					LCD_DrawPoint(x+(x0+i*8),y+y0,backColor);  //�豳����ɫ
				}
				dat<<=1;                      //���θ�λ������Ϊ���λ     
			}
			
		}
	}
}

/**************************************************************************
***** ��������  void Draw_Text_8_16_Str(u32 x,u32 y,u16 backColor,u16 color,const unsigned char *chs)
***** ���ܣ�    ���ƴ�СΪ16��8��16x16���ַ���
***** ������    x��������   y��������     color������ɫ  
   	            backColor  ���ֱ�����ɫ  ch����ģ����
***** ����ֵ��	��
****************************************************************************/
void Draw_Text_8_16_Str(u32 x,u32 y,u16 backColor,u16 color,const unsigned char *chs)
{
	u32 i=0;
	while (*chs != '\0')
	{
		if (*chs > 0xa0)				//�������붼�Ǵ���0xa0  (��������������֮���ת��)
		{
			for (i=0 ;i < GB16_NUM();i++)  //GB16_NUM������������ʾ���ֿ��ڵĸ���
			{	
				if(x>235){x=0;y=y+16;}

				if ((*chs == hz_index[i*2]) && (*(chs+1) == hz_index[i*2+1]))
				{
					show_xXx(x,y,backColor,color,&hz[32*(i)],16,16);
					x +=16;
					break;
				}
			}
			chs+=2;
		}

		else	 //������ַ�ִ������
		{	if(x>230){x=0;y=y+16;}
			for (i=0 ;i < ASCII_NUM();i++)
			{
				if (*chs == zf_index[i])
				{
					show_xXx(x,y,backColor,color,&zf[16*i],16,8);
					x += 8;
					break;
				}
			}
			chs++;
		}	 
	}
}
#if LCD_ZK_EN
/****************************************************
��������Draw_Text_16_16_Str_zk
�βΣ�x��������   y��������     color������ɫ  
   	  backColor  ���ֱ�����ɫ  chs����ģ����
����ֵ��
�������ܣ������ֿ���ʾ���֣�16*16��
****************************************************/
#define FONT_SIZE   (16)
#define HZ_INDEX(hz)    ((hz[0] - 0xa1) * 94 + (hz[1] - 0xa1))  
#define DOTS_BYTES  (FONT_SIZE * FONT_SIZE / 8) 

unsigned char dots[DOTS_BYTES]; 
u32 zk_addr1 = 0;
void Draw_Text_16_16_Str_zk(u32 x,u32 y,u16 backColor,u16 color,const unsigned char *chs)
{
	while (*chs != '\0')
	{
		zk_addr1 = HZ_INDEX(chs) * DOTS_BYTES;//�ҵ�Flash�����ݵ�ַ
		FLASH_ReadBuffer(dots,zk_addr1,DOTS_BYTES);//����Flash�е�����
		if(x>235){x=0;y=y+16;}
		show_xXx(x,y,backColor,color,dots,16,16);
		x +=16;
		chs+=2;
	}
}
/****************************************************
��������Draw_Text_Str
�βΣ�x��������   y��������     color������ɫ  
   	  backColor  ���ֱ�����ɫ  chs����ģ����
����ֵ��
�������ܣ������ֿ���ʾ���֣�16*16�����ַ�(8*16)
****************************************************/
void Draw_Text_Str(u32 x,u32 y,u16 backColor,u16 color,const unsigned char *chs)
{
	u32 i=0;
	while (*chs != '\0')
	{
		if (*chs > 0xa0)				//�������붼�Ǵ���0xa0
		{
			zk_addr1 = HZ_INDEX(chs) * DOTS_BYTES;//�ҵ�Flash�����ݵ�ַ
			FLASH_ReadBuffer(dots,zk_addr1,DOTS_BYTES);//����Flash�е�����
			
			if(x>235){x=0;y=y+16;}
			show_xXx(x,y,backColor,color,dots,16,16);
			x +=16;
			chs+=2;
		}
		else	 //������ַ�ִ������
		{	if(x>230){x=0;y=y+16;}
			for (i=0 ;i < ASCII_NUM();i++)
			{
				if (*chs == zf_index[i])
				{
					show_xXx(x,y,backColor,color,&zf[16*i],16,8);
					x += 8;
					break;
				}
			}
			chs++;
		}	 
	}
}
#endif

/**************************************************************************
***** ��������   Paint_Bmp()
***** ���ܣ�     ��ʾָ����СͼƬ
***** ������     x0 ��ʼλ�ã�y0��ʼλ��  ��high�߶ȣ�width ��� ��bmp ͼƬ��������
***** ����ֵ��	 �� 
****************************************************************************/
void Paint_Bmp(int x0,int y0,int width,int high,const unsigned char bmp[])
{
	int x,y;
	u32 c;
	int p = 0;
	
    for( y = y0 ; y < (high+y0) ; y++ )
    {
    	for( x = x0 ; x < (width+x0) ; x++ )
    	{
    		c = bmp[p+1] | (bmp[p]<<8) ;
			if ( ( (x) < LCD_WIDTH) && ( (y) < LCD_HEIGHT) )
			LCD_DrawPoint(x,y,c);  
    		p = p + 2 ;
    	}
    }
}


/**************************************************************************
***** ��������   reverse()  
***** ���ܣ�     ��ת�ַ��� 
***** ������     char *s Ҫ��ת���ַ���
***** ����ֵ��	 ����ת���õ��ַ����׵�ַ 
****************************************************************************/
 
char *reverse(char *s)  
{  
    char temp;  
    char *p = s;    //pָ��s��ͷ��  
    char *q = s;    //qָ��s��β��  
    while(*q)  
        ++q;  
    q--;  
  
    //�����ƶ�ָ�룬ֱ��p��q����  
    while(q > p)  
    {  
        temp = *p;  
        *p++ = *q;  
        *q-- = temp;  
    }  
    return s;  
}  
  
/* 
 * ���ܣ�����ת��Ϊ�ַ��� 
 * char s[] �������Ǵ洢������ÿһλ 
 */ 
/**************************************************************************
***** ��������   my_itoa 
***** ���ܣ�     ����ת��Ϊ�ַ���
***** ������     int nҪת����������
***** ����ֵ��	 ����ת���õ��ַ����׵�ַ 
****************************************************************************/ 
char *my_itoa(int n)  
{  
    int i = 0,isNegative = 0;  
    static char s[20];      //����Ϊstatic������������ȫ�ֱ���  
    if((isNegative = n) < 0) //����Ǹ�������תΪ����  
    {  
        n = -n;  
    }  
    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת  
    {  
        s[i++] = n%10 + '0';  
        n = n/10;  
    }while(n > 0);  
  
    if(isNegative < 0)   //����Ǹ��������ϸ���  
    {  
        s[i++] = '-';  
    }  
    s[i] = '\0';    //�������ַ���������  
    return reverse(s);    
}

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//��ʾ��ɫ 	    
	} 
}

