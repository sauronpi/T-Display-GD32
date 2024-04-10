//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STC12C5A60S2,����30M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for C51
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/8/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
//********************************************************************************
//=========================================��Դ����================================================//
//VCC��DC 3.3V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI
//Һ����ģ��            ��Ƭ��
//  SDA          ��       P15        //SPIд�ź�
//=======================================Һ���������߽���==========================================//
//Һ����ģ��            ��Ƭ��
//  CS           ��       P13       //Ƭѡ�ź�
//  RES          ��       P33       //��λ�ź�
//  DC           ��       P12       //����/����ѡ������ź�
//  SCL          ��       P17       //SPIʱ���ź�
//  BLK          ��       P32       //��������źţ��������Ҫ���ƣ���3.3V
//=========================================����������=========================================//
//STC89C52RC��Ƭ��ROM̫С���޷����д��������ܵĳ������Դ���������Ҫ����
//��ʹ�ô�������ģ�鱾������������ɲ�����
//������ʹ�õ�������������ΪSPI
//������ģ��            ��Ƭ��
//  T_CLK        ��       P36       //������SPIʱ���ź�
//  T_CS         ��       P37       //������Ƭѡ�����ź�
//  T_DIN        ��       P34       //������SPIд�ź�
//  T_DO         ��       P35       //������SPI���ź�
//  T_IRQ        ��       P40       //��������Ӧ����źţ��絥Ƭ����P4�飬�����и�����������IO���޸Ĵ��붨��
//**************************************************************************************************/	
#include "sys.h"
#include "lcd.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

void  delay_ms_new(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void	SPI_init(void)	
{
	SPCTL = (SSIG << 7)+(SPEN << 6)+(DORD << 5)+(MSTR << 4)+(CPOL << 3)+(CPHA << 2)+SPEED_16;
}

/*****************************************************************************
 * @name       :void SPI_WriteByte(u8 byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using C51's Hardware SPI
 * @parameters :d:Data to be written
 * @retvalue   :None
******************************************************************************/
void SPI_WriteByte(u8 byte)
{			
			SPDAT = byte;		//����һ���ֽ�
			while((SPSTAT & SPIF)==0)	;	//�ȴ��������
			SPSTAT = SPIF+WCOL;			//��0 SPIF��WCOL��־
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 Reg)	 
{	
	LCD_CS=0;
	LCD_RS=0;
	SPI_WriteByte(Reg);
	LCD_CS=1;
} 

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 Data)
{
	LCD_CS=0;
	LCD_RS=1;
	SPI_WriteByte(Data);
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void LCD_WR_DATA_16Bit(u16 Data)
{
	LCD_CS=0;
	LCD_RS=1;
	SPI_WriteByte(Data>>8);
	SPI_WriteByte(Data);
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u8 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	u16 i,j;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
	LCD_CS=0;
	LCD_RS=1;
    for(i=0;i<lcddev.width;i++)
	{
	  for (j=0;j<lcddev.height;j++)
	   	{
        SPI_WriteByte(Color>>8);
				SPI_WriteByte(Color);
	    }
	}
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	    
} 	 

/*****************************************************************************
 * @name       :void LCD_Reset(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_Reset(void)
{	
	LCD_RESET=0;
	delay_ms(20);
	LCD_RESET=1;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{
	SPI_init(); //��ʼ��Ӳ��SPI
	LCD_Reset(); //��ʼ��֮ǰ��λ
//************* ST7789V��ʼ��**********//	
	delay_ms(120);                //ms
	LCD_WR_REG(0x11);     //Sleep out
	delay_ms(120);                //Delay 120ms
	LCD_WR_REG(0x36);     
	LCD_WR_DATA(0x00);  
	LCD_WR_REG(0x3A);     
	LCD_WR_DATA(0x05);  //0x05( 65K Color) 
	LCD_WR_REG(0x21);     
	LCD_WR_REG(0xB2);     
	LCD_WR_DATA(0x05);   
	LCD_WR_DATA(0x05);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x33);   
	LCD_WR_DATA(0x33);   
	LCD_WR_REG(0xB7);     
	LCD_WR_DATA(0x23);   
	LCD_WR_REG(0xBB);     
	LCD_WR_DATA(0x22);   
	LCD_WR_REG(0xC0);     
	LCD_WR_DATA(0x2C);   
	LCD_WR_REG(0xC2);     
	LCD_WR_DATA(0x01);   
	LCD_WR_REG(0xC3);     
	LCD_WR_DATA(0x13);   
	LCD_WR_REG(0xC4);     
	LCD_WR_DATA(0x20);   
	LCD_WR_REG(0xC6);     
	LCD_WR_DATA(0x0F);   
	LCD_WR_REG(0xD0);     
	LCD_WR_DATA(0xA4);   
	LCD_WR_DATA(0xA1);   
	LCD_WR_REG(0xD6);     
	LCD_WR_DATA(0xA1);   
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x70);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x2E);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x25);
	LCD_WR_DATA(0x2A);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x70);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x42);
	LCD_WR_DATA(0x42);
	LCD_WR_DATA(0x38);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x2C);
	LCD_WR_REG(0x29);     //Display on
	//	LCD_WR_REG(0x2A);     //Column Address Set
	//	LCD_WR_DATA(0x00);   
	//	LCD_WR_DATA(0x34);   //52
	//	LCD_WR_DATA(0x00);   
	//	LCD_WR_DATA(0xBA);   //186
	//	LCD_WR_REG(0x2B);     //Row Address Set
	//	LCD_WR_DATA(0x00);   
	//	LCD_WR_DATA(0x28);   //40
	//	LCD_WR_DATA(0x01);   
	//	LCD_WR_DATA(0x17);   //279
	//	LCD_WR_REG(0x2C);
	//	LCD_WR_REG(0x11);     //Sleep out
	//	delay_ms(120);                //Delay 120ms
	//	LCD_WR_REG(0x10);     //Sleep in
	//	delay_ms(120);                //Delay 120ms
	//����LCD���Բ���
	LCD_direction(USE_HORIZONTAL);//����LCD��ʾ���� 
	LCD_BL=1;//��������	 
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA((xStar+lcddev.xoffset)>>8);
	LCD_WR_DATA(xStar+lcddev.xoffset);		
	LCD_WR_DATA((xEnd+lcddev.xoffset)>>8);
	LCD_WR_DATA(xEnd+lcddev.xoffset);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA((yStar+lcddev.yoffset)>>8);
	LCD_WR_DATA(yStar+lcddev.yoffset);		
	LCD_WR_DATA((yEnd+lcddev.yoffset)>>8);
	LCD_WR_DATA(yEnd+lcddev.yoffset);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM				
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			lcddev.xoffset=52;
			lcddev.yoffset=40;
			LCD_WriteReg(0x36,0);//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=40;
			lcddev.yoffset=53;
			LCD_WriteReg(0x36,(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;
     		lcddev.xoffset=53;
			lcddev.yoffset=40;			
			LCD_WriteReg(0x36,(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=40;
			lcddev.yoffset=52;
			LCD_WriteReg(0x36,(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 





